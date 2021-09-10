//
// Created by ultimatenick on 10/08/21.
//

#include <cmath>

#include "glad/glad.h"
#include "gtc/matrix_transform.hpp"
#include "Model.hpp"
#include "Player.hpp"
#include "trigonometric.hpp"
#include "Sphere.hpp"
#include "CubeWireframe.hpp"
#include "Text2D.hpp"
#include "Item.hpp"
#include "stb_image.h"
#include "ActionHandler.hpp"
#include "fmt/format.h"
#include "Shader.hpp"

float Model::get_day_time() const {
    if (day_length <= 0) {
        return 0.5;
    }
    float t;
    t = glfwGetTime();
    t = t / day_length;
    t = t - (int)t;
    return t;
}

float Model::get_daylight() const {
    float timer = get_day_time();
    if (timer < 0.5) {
        float t = (timer - 0.25) * 100;
        return 1 / (1 + powf(2, -t));
    }
    else {
        float t = (timer - 0.85) * 100;
        return 1 - 1 / (1 + powf(2, -t));
    }
}

Sphere Model::sky{1,3};

int Model::get_scale_factor() const {
    int window_width, window_height;
    int buffer_width, buffer_height;

    // screen coordinates
    glfwGetWindowSize(window, &window_width, &window_height);
    // sizes in pixels
    glfwGetFramebufferSize(window, &buffer_width, &buffer_height);

    int result = buffer_width / window_width;
    result = std::max(1, result);
    result = std::min(2, result);
    return result;
}

bool Model::is_flying() const{
    return flying;
}

int Model::getWidth() const {
    return width;
}

int Model::getHeight() const {
    return height;
}

int Model::getScale() const {
    return scale;
}

void Model::set_player(const glm::vec3& position, const glm::vec2& rotation, std::string_view name, int id) {
    player.reset(new Player{*this, name, id, position, rotation});
}

void Model::delete_player() {
    player.reset(nullptr);
}

// TODO to be tested
bool Model::chunk_visible(const glm::ivec2 &pq) {
    int x = pq.x * Chunk::getSize() - 1;
    int z = pq.y * Chunk::getSize() - 1;
    int d = Chunk::getSize() + 1;

    const Chunk& c = get_chunk_at(pq);
    int miny = c.getMinY();
    int maxy = c.getMaxY();

    std::array<glm::vec3, 8> points{{
            {x + 0, miny, z + 0},
            {x + d, miny, z + 0},
            {x + 0, miny, z + d},
            {x + d, miny, z + d},
            {x + 0, maxy, z + 0},
            {x + d, maxy, z + 0},
            {x + 0, maxy, z + d},
            {x + d, maxy, z + d}
    }};

    for(const auto& p : points){
        if(player->getFrustum().is_inside(p)){
            return true;
        }
    }
    return false;
}

constexpr glm::ivec2 Model::chunked(const glm::vec3& position) {
    return {
        chunked(position.x),
        chunked(position.z)
    };
}

int Model::highest_block(const glm::vec2& pq) {
    const Chunk& chunk = chunks.at(pq);
    return chunk.getMaxY();
}

const std::unordered_map<glm::ivec2, Chunk> &Model::getChunks() const {
    return chunks;
}

int Model::get_chunk_distance(const glm::ivec2 &pq1, const glm::ivec2 &pq2) {
        auto delta = pq1 - pq2;
        return glm::max(glm::abs(delta.x), glm::abs(delta.y));
}

void Model::set_block(const glm::ivec3 &pos, const TileBlock &w) {
    glm::ivec2 pq = chunked(pos);
    int p = pq.x;
    int q = pq.y;

    get_chunk_at(pq).set_block(pos, w);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            if (dx == 0 && dz == 0) {
                continue;
            }
            if (dx && chunked(pos.x + dx) == p) {
                continue;
            }
            if (dz && chunked(pos.z + dz) == q) {
                continue;
            }
            get_chunk_at({p + dx, q + dz}).set_block({pos.x, pos.y, pos.z}, TileBlock{0});
        }
    }
}

Chunk &Model::get_chunk_at(const glm::ivec2 &pq) {
    if (!chunks.contains(pq)) {
        const auto &inserted_pair = chunks.emplace(pq, Chunk{*this, pq}, false).first;
        return inserted_pair->second;
    }
    return chunks[pq]
}

constexpr int Model::chunked(int val) {
    return glm::floor(glm::round(val) / Chunk::getSize());
}

void Model::record_block(Block block) {
    blocks[1] = blocks[0];
    blocks[0] = block;
}

TileBlock Model::get_block(const glm::ivec3 position) {
    auto result = chunks.find(chunked(position));
    return (result != chunks.end()) ? result->second : TileBlock{};
}

void Model::builder_block(const glm::ivec3 &pos, BlockType w)  {
    if (pos.y <= 0 || pos.y >= 256) {
        return;
    }
    if (get_block(pos).is_destructable() && w != BlockType::CLOUD) {
        set_block(pos, TileBlock{});
    }
}

void Model::render_chunks() const {
    if(!player || !shaders.block_shader.get_id()){
        return;
    }
    const Shader& shader = shaders.block_shader;
    shader.use();

    glm::ivec2 player_pq = player->get_pq();
    int light = get_daylight();

    shader.set_viewproj(get_viewproj(proj_type::PERSP));
    shader.set_camera(player->get_position());
    shader.set_sampler(0);
    shader.set_timer(get_day_time());
    shader.set_extra(1, 2);
    shader.set_extra(2, light);
    shader.set_extra(3, render_radius * Chunk::size);
    shader.set_extra(4, ortho);

    for(const auto& c : chunks){
        if(c.is_visible(player->getFrustum()) && get_chunk_distance(player_pq, c.pq) < RENDER_CHUNK_RADIUS){
            c.render();
        }
    }
}

float Model::getFov() const {
    return fov;
}

// TODO to be completed
void Model::render_sky() const {
    if(!player){
        return;
    }
    const Shader& shader{shaders.sky_shader};
    shader.set_viewproj(get_viewproj(proj_type::PERSP));
    shader.set_sampler(2);
    shader.set_timer(get_day_time());
    // TODO switch to render object
    sky.render();
}

glm::mat4 Model::get_viewproj(proj_type pt) const {
    const glm::mat4 view{
            glm::lookAt(player->get_position(),
            player->get_position() + player->get_camera_direction_vector(),
            {0, 1, 0})
    };

    switch (pt) {
        case proj_type::PERSP:
            return persp_proj * view;
        break;
        case proj_type::ORTHO_2D:
            return ortho_proj_2d * view;
        break;
    }
}

void Model::render_wireframe() {
    if(!player){
        return;
    }
    std::pair<glm::ivec3, TileBlock> hit_info = player->hit_test(false);
    if(hit_info.second.is_obstacle()){
        Shader& s = shaders.line_shader;
        glLineWidth(1);
        glEnable(GL_COLOR_LOGIC_OP);
        s.use();
        s.set_viewproj(get_viewproj(proj_type::PERSP));
        CubeWireframe{hit_info.first}.render_lines();
        glDisable(GL_COLOR_LOGIC_OP);
    }
}

void Model::render_crosshair() {
    const Shader& shader = shaders.line_shader;

    shader.use();
    glLineWidth(4 * scale);
    glEnable(GL_COLOR_LOGIC_OP);
    shader.set_viewproj(get_viewproj(proj_type::ORTHO_2D));
    crosshair.render_lines();
}

void Model::render_text(int justify, const glm::vec2 &position, float n, std::string_view text) {
    const Shader& shader = shaders.text_shader;

    shader.use();
    shader.set_viewproj(get_viewproj(proj_type::ORTHO_2D));
    shader.set_sampler(1);
    const glm::vec2 justified_position{position - glm::vec2{n * justify * (text.size() - 1) / 2, 0}};
    Text2D{justified_position, n, text}.render_object();
}

void Model::render_item() {
    const Shader& shader = shaders.block_shader;
    shader.use();
    shader.set_viewproj(get_viewproj(proj_type::PERSP));
    shader.set_camera({0,0,5});
    shader.set_sampler(0);
    shader.set_timer(get_day_time());

    Item{actual_item}.render_object();
}

void Model::record_block(const glm::ivec3 &pos, const TileBlock &w) {
    record_block(Block{pos, w});
}

void Model::set_block(const glm::ivec3 &pos) {
    set_block(pos, actual_item);
}

void Model::record_block(const glm::ivec3 &pos) {
    record_block(pos, actual_item);
}

void Model::set_actual_item(TileBlock item) {
    actual_item = item;
}

void Model::switch_flying() {
    flying = !flying;
}

void Model::set_next_item() {
    actual_item = TileBlock::items[(actual_item.getIndex() + 1) % TileBlock::items.size()];
}

void Model::set_prev_item() {
    int new_index = actual_item.getIndex() - 1;
    if(new_index < 0) {
        new_index = static_cast<int>(TileBlock::items.size()) - 1;
    }
    actual_item = TileBlock::items[new_index];
}

Model::Model() : Model({"block_vertex.vs", "block_fragment.fs"},
                       {"line_vertex.vs", "line_fragment.fs"},
                       {"sky_vertex.vs", "sky_fragment.fs"},
                       {"text_vertex.vs", "text_fragment.fs"})
{}

GLFWwindow * Model::create_window(bool is_fullscreen) {
    GLFWmonitor *monitor = nullptr;

    if(is_fullscreen){
        int mode_count;
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *modes = glfwGetVideoModes(monitor, &mode_count);
        width = modes[mode_count - 1].width;
        height = modes[mode_count - 1].height;
    }

    return glfwCreateWindow(width, height, "Craft Cpp", monitor, nullptr);
}

Model::~Model() {
    glfwTerminate();
}

GLFWwindow *Model::get_window() {
    return window;
}

void Model::set_ortho(int ortho_size) {
    ortho = ortho_size;
}

void Model::set_fov(int fov_degrees) {
    fov = fov_degrees;
}

Player *Model::get_player() const{
    return player.get();
}

Model::Model(const Shader &block_shader, const Shader &line_shader, const Shader &sky_shader, const Shader &text_shader) :
    shaders{block_shader, line_shader, sky_shader, text_shader},
    width{WINDOW_WIDTH},
    height{WINDOW_HEIGTH}
    {
    glfwSetTime(day_length / 3.0);
    window = create_window(FULLSCREEN);
    previous_timestamp = glfwGetTime();

    if(window) {
        set_player({}, {}, "player_0", 0);
        ActionHandler::initialize(this);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(VSYNC);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ActionHandler::set_callbacks(window);
    }
}

void Model::update_window_size() {
    scale = get_scale_factor();
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    update_proj_matrix();
}

void Model::handle_input(double dt) {
    ActionHandler::handle_mouse_input();
    ActionHandler::handle_movement(dt);
}

void Model::render_scene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    render_sky();
    glClear(GL_DEPTH_BUFFER_BIT);
    render_chunks();
    if(SHOW_WIREFRAME){
        render_wireframe();
    }
    glClear(GL_DEPTH_BUFFER_BIT);
    if (SHOW_CROSSHAIRS) {
        render_crosshair();
    }
    if (SHOW_ITEM) {
        render_item();
    }
    if(SHOW_INFO_TEXT){
        float ts = 12 * scale;
        float tx = ts / 2;
        float ty = height - ts;

        int hour = get_day_time() * 24;
        char am_pm = hour < 12 ? 'a' : 'p';
        hour = hour % 12;
        hour = hour ? hour : 12;

        int p = player->get_pq().x;
        int q = player->get_pq().y;
        float x = player->get_position().x;
        float y = player->get_position().y;
        float z = player->get_position().z;

        std::string_view s{
            fmt::format("(%d, %d) (%.2f, %.2f, %.2f) n_chunks: %d, hour: %d%cm",
                        p, q, x, y, z, chunks.size(), hour, am_pm);
        };

        render_text(ALIGN_LEFT, {tx, ty}, ts, s);
    }
}

bool Model::swap_pool() {
    glfwSwapBuffers(window);
    glfwPollEvents();
    return !glfwWindowShouldClose(window);
}

// build chunks around the player to test collisions
void Model::load_collision_chunks() {
    if(!Player){
        return;
    }
    const glm::ivec2& player_chunk = player->get_pq();
    const int r = CREATE_CHUNK_RADIUS;
    for(int dp = -r ; dp <= r ; dp++){
        for(int dq = -r ; dq <= r ; dq++) {
            glm::vec2 pos{player_chunk.x + dp, player_chunk.x + dq};
            if(!chunks.contains(pos)) {chunks.emplace(pos, Chunk{*this, pos, true});}
        }
    }
}

void Model::update_proj_matrix() {
    persp_proj = glm::perspective(glm::radians(fov), static_cast<float>(width) / (height), z_near, z_far);
    ortho_proj_2d = glm::ortho(0, width, 0, height, -1, 1);
}

void Model::load_visible_chunks() {
    if(!player){
        return;
    }
    glm::ivec2 pq = player->get_pq();
    for(int dp = -RENDER_CHUNK_RADIUS ; dp <= RENDER_CHUNK_RADIUS ; dp++ ){
        for(int dq = -RENDER_CHUNK_RADIUS ; dq <= RENDER_CHUNK_RADIUS ; dq++ ){
            Chunk c{*this, {pq.x + dq, pq.y + dq}, false};
            if(c.is_visible(player->getFrustum()) && !chunks.contains(c.pq)){
                c.init();
                chunks.insert({c.pq, c})
            }
        }
    }
}

void Model::remove_distant_chunks() {
    if(!player || chunks.size() < MAX_CHUNKS){
        return;
    }
    glm::ivec2 player_pq = player->get_pq();
    // position-chunk pair
    for(const auto& pq_c_pair : chunks){
        if(get_chunk_distance(player_pq, pq_c_pair.first) > DELETE_CHUNK_RADIUS){
            // every modification of chunk is lost
            chunks.erase(pq_c_pair.first);
        }
    }
}

void Model::update_chunk_map() {
    remove_distant_chunks();
    load_collision_chunks();
    load_visible_chunks();
}

bool Model::loop() {
    update_window_size();
    double now = glfwGetTime();
    double dt = now - previous_timestamp;

    dt = glm::min(dt, 0.2);
    dt = glm::max(dt, 0.0);
    previous_timestamp = now;

    update_chunk_map();
    handle_input(dt);
    render_scene();

    return swap_pool();
}
