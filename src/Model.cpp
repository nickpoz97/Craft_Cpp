//
// Created by ultimatenick on 10/08/21.
//

#include <cmath>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"
#include "Player.hpp"
#include "Model.hpp"
#include "trigonometric.hpp"
#include "Sphere.hpp"
#include "CubeWireframe.hpp"
#include "Text2D.hpp"
#include "Item.hpp"
#include "stb_image.h"
#include "ActionHandler.hpp"
#include "fmt/format.h"
#include "Shader.hpp"
#include "Chunk.hpp"

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

const Sphere Model::sky{1,3};

int Model::get_scale_factor(int width, int height) {
    int result = width / height;
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
    player.reset(new Player{name, id, position, rotation});
}

void Model::delete_player() {
    player.reset(nullptr);
}

int Model::highest_block(const glm::vec2& pq) {
    const Chunk& chunk = chunks.at(pq);
    return chunk.get_max_y();
}

const std::unordered_map<glm::ivec2, Chunk> &Model::getChunks() const {
    return chunks;
}

void Model::set_block(const glm::ivec3 &pos, BlockType w) {
    glm::ivec2 pq = Chunk::chunked(pos);
    int p = pq.x;
    int q = pq.y;

    get_chunk_at(pq).set_block(pos, w);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            if (dx == 0 && dz == 0) {
                continue;
            }
            if (dx && Chunk::chunked(pos.x + dx) == p) {
                continue;
            }
            if (dz && Chunk::chunked(pos.z + dz) == q) {
                continue;
            }
            get_chunk_at({p + dx, q + dz}).set_block({pos.x, pos.y, pos.z}, BlockType::EMPTY);
        }
    }
}

Chunk &Model::get_chunk_at(const glm::ivec2 &pq){
    // need to create new chunk if not present
    // inserted pair is an iterator to the already present element or the new one
    const auto &inserted_pair{chunks.try_emplace(pq, Chunk{pq, false}).first};
    // Chunk object is returned
    return inserted_pair->second;
}

/*void Model::record_block(Block block) {
    blocks[1] = blocks[0];
    blocks[0] = block;
}*/

TileBlock Model::get_block(const glm::ivec3 position) {
    const Chunk& chunk = get_chunk_at(Chunk::chunked(position));
    return (chunk) ? chunk.get_block(position) : TileBlock{};
}

void Model::builder_block(const glm::ivec3 &pos, BlockType w = BlockType::EMPTY)  {
    if (pos.y <= 0 || pos.y >= 256) {
        return;
    }
    if (get_block(pos).is_destructable() && w != BlockType::CLOUD) {
        set_block(pos, w);
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
    shader.set_extra(3, RENDER_CHUNK_RADIUS * Chunk::SIZE);
    shader.set_extra(4, ortho);

    for(const auto& pq_c : chunks){
        const Chunk& c = pq_c.second;
        proj_type pt = ortho ? proj_type::ORTHO_3D : proj_type::PERSP;

        if(c.is_visible(get_viewproj(pt)) && get_chunk_distance(get_player_chunk(), c) < RENDER_CHUNK_RADIUS){
            c.render_object();
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
    sky.render_object();
}

glm::mat4 Model::get_viewproj(proj_type pt) const {
    const glm::mat4 view{
            glm::lookAt(player->get_position(),
            player->get_position() + player->get_camera_direction_vector(),
            {0, 1, 0})
    };

    // TODO insert ORTO_3D
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
    Block hit_block = player_hit_test(false);
    if(hit_block.w.is_obstacle()){
        const Shader& s = shaders.line_shader;
        glLineWidth(1);
        glEnable(GL_COLOR_LOGIC_OP);
        s.use();
        s.set_viewproj(get_viewproj(proj_type::PERSP));
        CubeWireframe{hit_block.position}.render_lines();
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

/*void Model::record_block(const glm::ivec3 &pos) {
    record_block(pos, actual_item);
}*/

void Model::set_actual_item(BlockType item_type) {
    actual_item = item_type;
}

void Model::switch_flying() {
    flying = !flying;
}

void Model::set_next_item() {
    actual_item = TileBlock::items[(actual_item + 1) % TileBlock::items.size()];
}

void Model::set_prev_item() {
    int new_index = actual_item - 1;
    if(new_index < 0) {
        new_index = TileBlock::items.size() - 1;
    }
    actual_item = TileBlock::items[new_index];
}

Model::Model() : Model({"block_vertex.vs", "block_fragment.fs", {}},
                       {"line_vertex.vs", "line_fragment.fs", {}},
                       {"sky_vertex.vs", "sky_fragment.fs", {}},
                       {"text_vertex.vs", "text_fragment.fs", {}})
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
    height{WINDOW_HEIGTH},
    crosshair{WINDOW_WIDTH, WINDOW_HEIGTH, get_scale_factor(WINDOW_WIDTH, WINDOW_HEIGTH)}
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
    update_proj_matrices();
}

void Model::update_window_size() {
    scale = get_scale_factor(0, 0);
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    update_proj_matrices();
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
                        p, q, x, y, z, chunks.size(), hour, am_pm)
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
    const glm::ivec2& player_chunk = player->get_pq();
    const int r = CREATE_CHUNK_RADIUS;
    for(int dp = -r ; dp <= r ; dp++){
        for(int dq = -r ; dq <= r ; dq++) {
            glm::vec2 pq_coordinate{player_chunk.x + dp, player_chunk.x + dq};
            chunks.try_emplace(pq_coordinate, Chunk{pq_coordinate, true});
        }
    }
}

void Model::update_proj_matrices() {
    persp_proj = glm::perspective(glm::radians(fov), static_cast<float>(width) / (height), z_near, z_far);
    ortho_proj_2d = glm::ortho(0, width, 0, height, -1, 1);
}

void Model::load_visible_chunks() {
    glm::ivec2 pq = player->get_pq();
    for(int dp = -RENDER_CHUNK_RADIUS ; dp <= RENDER_CHUNK_RADIUS ; dp++ ){
        for(int dq = -RENDER_CHUNK_RADIUS ; dq <= RENDER_CHUNK_RADIUS ; dq++ ){
            proj_type pt = ortho ? proj_type::ORTHO_3D : proj_type::PERSP;
            // build chunk and check if it is in a visible position
            Chunk c{{pq.x + dq, pq.y + dq}, false};
            if(c.is_visible(get_viewproj(pt))){
                // check if chunk was not already in the map
                bool inserted = chunks.insert({c.pq, c}).second;
                // if abstent -> init it
                if(inserted) {c.init_chunk();}
            }
        }
    }
}

void Model::remove_distant_chunks() {
    if(chunks.size() < MAX_CHUNKS){
        return;
    }
    glm::ivec2 player_pq = player->get_pq();
    // position-chunk pair
    for(const auto& pq_c_pair : chunks){
        if(get_chunk_distance(get_player_chunk(), pq_c_pair.second) > DELETE_CHUNK_RADIUS){
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

std::array<const Chunk*, 6> Model::chunk_neighbors_pointers(const glm::ivec2& pq) const{
    std::array<const Chunk*, 6> neighbors{};
    auto it  = neighbors.begin();

    for(int dp = -1 ; dp <= 1 ; dp++){
        for(int dq = -1 ; dq <= 1 ; dq++){
            *(it++) = & chunks.at({pq.x - dp, pq.y - dq});
        }
    }

    return neighbors;
}

const Chunk & Model::get_player_chunk() const {
    return get_chunk_at(player->get_pq());
}

const Chunk &Model::get_chunk_at(const glm::ivec2 &pq) const {
    return chunks.at(pq);
}

Block Model::player_hit_test(bool previous) const {
    Block result{};
    float best{};

    for(const auto& c_ref : chunk_neighbors_pointers(player->get_pq())){
        const Chunk& c = *(c_ref);

        Block hit_block = player->ray_hit(c, previous, 8);
        if(hit_block.w.is_obstacle()){
            float distance = glm::distance(player->get_position(), glm::vec3{hit_block.position});
            if(best == 0.0f || distance < best){
                best = distance;
                result = hit_block;
            }
        }
    }
    return result;
}

std::pair<bool, glm::vec3> Model::collide(int height, const std::unordered_map<glm::ivec2, Chunk> &chunk_map) {
    const glm::vec3& position{player->get_position()};
    glm::ivec2 pq{Chunk::chunked(position)};
    const Chunk& c{chunk_map.at(pq)};
    glm::vec3 collision_point{};
    bool result{};

    // TODO check if truncation is better than rounding
    const glm::ivec3 int_pos{position};
    const glm::vec3 decimal_dif_pos(position - static_cast<glm::vec3>(int_pos));
    float pad = 0.25;

    // TODO check (+1 -> value > pad) and (-1 -> value < pad)
    glm::ivec3 signs{glm::step(pad, decimal_dif_pos) - glm::step(pad, -decimal_dif_pos)};
    std::array<glm::vec3, 3> block_pos{{
                                               {int_pos.x + (signs.x) * 1, int_pos.y, int_pos.z},
                                               {int_pos.x, int_pos.y + (signs.y) * 1, int_pos.z},
                                               {int_pos.x, int_pos.y, int_pos.z + (signs.z) * 1},
                                       }};

    for(int y_step = 0; y_step < height ; ++y_step){
        glm::bvec3 enable{
                c.get_block(block_pos[0] + glm::vec3{signs.x, 0, 0} * pad).is_obstacle(),
                c.get_block(block_pos[1] + glm::vec3{0, signs.y, 0} * pad).is_obstacle(),
                c.get_block(block_pos[2] + glm::vec3{0, 0, signs.z} * pad).is_obstacle()
        };

        collision_point.x = (enable.x) ? int_pos.x + signs.x * pad : collision_point.x;
        collision_point.y = (enable.y) ? (result=1, int_pos.x) + signs.x * pad : collision_point.y;
        collision_point.z = (enable.z) ? int_pos.x + signs.x * pad : collision_point.z;
    }
    return {result, collision_point};
}

bool Model::test_point_visibility(const glm::vec3& point) const{
    proj_type pt = ortho ? proj_type::ORTHO_3D : proj_type::PERSP;
    glm::vec4 clip_coords = get_viewproj(pt) * glm::vec4{point, 1.0};

    glm::vec4 abs_cc = glm::abs(clip_coords);
    float w_coord = clip_coords.w;

    return abs_cc.x < w_coord && abs_cc.y < w_coord && abs_cc.z < w_coord;
}

Crosshair Model::get_crosshair(int width, int height, int scale) {
    return Crosshair(width, height, scale);
}
