//
// Created by ultimatenick on 10/08/21.
//
#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"
#include "Player.hpp"
#include "Model.hpp"
#include "../Geometry/Sphere.hpp"
#include "../Geometry/CubeWireframe.hpp"
#include "../Geometry/Text2D.hpp"
#include "../Geometry/Item.hpp"
#include "stb_image.h"
#include "ActionHandler.hpp"
#include "fmt/format.h"
#include "../Rendering/Shader.hpp"
#include "../Geometry/Chunk.hpp"
#include "GameView.hpp"
#include "../Rendering/GLError.hpp"

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

bool Model::is_flying() const{
    return flying;
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
    using proj_type = GameView::proj_type;
    const Shader& shader = shaders.block_shader;
    shader.use();

    glm::ivec2 player_pq = player->get_pq();
    int light = get_daylight();

    shader.set_viewproj(get_viewproj(proj_type::PERSP));
    shader.set_camera(player->get_position());
    shader.set_sampler(0);
    shader.set_timer(get_day_time());
    shader.set_extra_uniform("sky_sampler", 2);
    shader.set_extra_uniform("daylight", light);
    shader.set_extra_uniform("fog_distance", RENDER_CHUNK_RADIUS * Chunk::SIZE);
    shader.set_extra_uniform("ortho", game_view.get_ortho());

    for(const auto& pq_c : chunks){
        const Chunk& c = pq_c.second;
        proj_type pt = game_view.get_ortho() ? proj_type::ORTHO_3D : proj_type::PERSP;

        //if(c.is_visible(get_viewproj(pt)) && get_chunk_distance(get_player_chunk(), c) < RENDER_CHUNK_RADIUS){
            c.render_object(chunk_neighbors_pointers(c.pq));
        //}
    }
}

void Model::render_sky() const {
    const Shader& shader{shaders.sky_shader};
    shader.use();
    shader.set_viewproj(get_viewproj(GameView::proj_type::PERSP));
    shader.set_sampler(2);
    shader.set_timer(get_day_time());
    sky.render_object();
}

glm::mat4 Model::get_viewproj(GameView::proj_type pt) const {
    using proj_type = GameView::proj_type;

    if(pt == proj_type::ORTHO_3D || pt == proj_type::PERSP) {

        return player->get_view_matrix() * game_view.get_proj_matrix(pt);
    }

    // view independence for UI
    return game_view.get_proj_matrix(pt);
}

void Model::render_wireframe() {
    Block hit_block = player_hit_test(false);
    if(hit_block.w.is_obstacle()){
        const Shader& s = shaders.line_shader;
        s.use();
        glLineWidth(1);
        glEnable(GL_COLOR_LOGIC_OP);
        s.use();
        s.set_viewproj(get_viewproj(GameView::proj_type::PERSP));
        CubeWireframe{hit_block.position}.render_lines();
        glDisable(GL_COLOR_LOGIC_OP);
    }
}

void Model::render_crosshair() {
    const Shader& shader = shaders.line_shader;

    shader.use();
    glLineWidth(4 * game_view.get_scale());
    glEnable(GL_COLOR_LOGIC_OP);
    shader.set_viewproj(get_viewproj(GameView::proj_type::UI));
    crosshair.render_lines();
    glDisable(GL_COLOR_LOGIC_OP);
}

void Model::render_text(int justify, const glm::vec2 &position, float n, std::string_view text) {
    const Shader& shader = shaders.text_shader;

    shader.use();
    shader.set_viewproj(game_view.get_proj_matrix(GameView::proj_type::UI));
    // TODO check if it is the right sampler
    shader.set_sampler(1);
    const glm::vec2 justified_position{position - glm::vec2{n * justify * (text.size() - 1) / 2, 0}};
    Text2D text2d{justified_position, n, text};
    text2d.render_object();
}

void Model::render_item() {
    const Shader& shader = shaders.block_shader;
    shader.use();
    shader.set_viewproj(get_viewproj(GameView::proj_type::ITEM));
    shader.set_camera({0,0,5});

    shader.set_sampler(0);
    shader.set_extra_uniform("ortho", 1);
    shader.set_timer(get_day_time());

    float size = 64 * game_view.get_scale();
    auto get_offset = [size](float axis){return 1 - size / axis * 2;};

    glm::vec2 offset{get_offset(game_view.get_width()),get_offset(game_view.get_height())};

    Item item_geometry{actual_item, -offset};
    item_geometry.render_object();
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

GLFWwindow *Model::get_window() {
    return game_view.get_window();
}

Player *Model::get_player() const{
    return player.get();
}

Model::Model(const Shader &block_shader, const Shader &line_shader, const Shader &sky_shader,
             const Shader &text_shader, GameView &game_view) :
    game_view{game_view},
    shaders{block_shader, line_shader, sky_shader, text_shader},
    crosshair{WINDOW_WIDTH, WINDOW_HEIGTH, game_view.get_scale()},
    sky{1,3}
    {
    glfwSetTime(day_length / 3.0);
    previous_timestamp = glfwGetTime();

    set_player({}, {}, "player_0", 0);
    ActionHandler::initialize(this);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(.7, .7, 1, 1);
#ifdef DEBUG
    fmt::print("ActionHandler enabled: {}", ActionHandler::is_enabled_for(this));
#endif
}

void Model::handle_input(double dt) {
    ActionHandler::handle_mouse_position();
    ActionHandler::handle_key_pressing(dt);
}

void Model::render_scene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //render_sky();
    render_chunks();
    if(SHOW_WIREFRAME){
        render_wireframe();
    }
    if (SHOW_CROSSHAIRS) {
        render_crosshair();
    }
    if (SHOW_ITEM) {
        render_item();
    }
    if(SHOW_INFO_TEXT){
        float ts = 12 * game_view.get_scale();
        float tx = ts / 2;
        float ty = game_view.get_height() - ts;

        int hour = get_day_time() * 24;
        char am_pm = hour < 12 ? 'a' : 'p';
        hour = hour % 12;
        hour = hour ? hour : 12;

        int p = player->get_pq().x;
        int q = player->get_pq().y;
        float x = player->get_position().x;
        float y = player->get_position().y;
        float z = player->get_position().z;
        //int p{}, q{}, x{}, y{}, z{};

        std::string s{
            // TODO float must be .2f
            fmt::format("pq: ({}, {}) position: ({}, {}, {}) n_chunks: {}, hour: {}{}m",
                        p, q, x, y, z, chunks.size(), hour, am_pm)
        };

        //fmt::print("{}\n", s);
        render_text(ALIGN_LEFT, {tx, ty}, ts, s);
    };
}

void Model::swap_pool() {
    glfwSwapBuffers(game_view.get_window());
    glfwPollEvents();
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

void Model::load_visible_chunks() {
    using proj_type = GameView::proj_type;

    glm::ivec2 pq = player->get_pq();
    for(int dp = -RENDER_CHUNK_RADIUS ; dp <= RENDER_CHUNK_RADIUS ; dp++ ){
        for(int dq = -RENDER_CHUNK_RADIUS ; dq <= RENDER_CHUNK_RADIUS ; dq++ ){
            proj_type pt = game_view.get_ortho() ? proj_type::ORTHO_3D : proj_type::PERSP;
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

void Model::loop() {
    double now = glfwGetTime();
    double dt = now - previous_timestamp;

    dt = glm::min(dt, 0.2);
    dt = glm::max(dt, 0.0);
    previous_timestamp = now;

    update_chunk_map();
    handle_input(dt);
    render_scene();

    swap_pool();
}

std::array<const Chunk*, 6> Model::chunk_neighbors_pointers(const glm::ivec2& pq) const{
    std::array<const Chunk*, 6> neighbors{};
    auto it  = neighbors.begin();

    for(int dp = -1 ; dp <= 1 ; dp++){
        for(int dq = -1 ; dq <= 1 ; dq++){
            auto result_it{chunks.find({pq.x - dp, pq.y - dq})};
            *(it++) = (result_it != chunks.end()) ? &(result_it->second) : nullptr;
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

Crosshair Model::get_crosshair(int width, int height, int scale) {
    return Crosshair(width, height, scale);
}

void Model::set_perspective_properties(int fov, int orto) {
    game_view.set_fov(fov);
    game_view.set_ortho(orto);
}

void Model::update_window() {
    // TODO update also item and others
    game_view.update();
    crosshair.update(game_view.get_width(), game_view.get_height(), game_view.get_scale());
}

int Model::load_texture(std::string_view path, GLint clamp_type) {
    unsigned texture_id;
    glGenTextures(1, &texture_id);

    static unsigned texture_index = 0;
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp_type);

    stbi_set_flip_vertically_on_load(true); // st coords instead of uv coords
    int width, height, nr_channels;
    unsigned char* data = stbi_load(path.data(), &width, &height, &nr_channels, 0);

    if(!data){
        stbi_image_free(data);
        return -1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    ++texture_index;
    return 0;
}

void Model::resolve_keyboard_input(int key, int control, bool cursor_disabled) {
    switch(key){
        case GLFW_KEY_ESCAPE:
            if(cursor_disabled) {glfwSetInputMode(game_view.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);}
            return;
        case GLFW_KEY_ENTER:
            control ? on_right_click() : on_left_click();
            return;
        case GLFW_KEY_TAB:
            switch_flying();
            return;
        case '0':
            set_actual_item(BlockType::GLASS);
            return;
        case 'E':
            set_next_item();
            return;
        case 'R':
            set_prev_item();
            return;
        default:
            if (key >= '1' && key <= '9') {
                // +1 to skip empty
                set_actual_item(TileBlock::items[key - '1']);
            }
            return;
    }
}

void Model::on_left_click() {
    Block result = player_hit_test(false);
    const auto& hit_pos {result.position};
    const auto& hit_tile_block {result.w};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){
        builder_block(hit_pos, BlockType::EMPTY);
        //model_p->record_block(hit_pos, BlockType::EMPTY);
        if(!get_block(hit_pos + glm::ivec3{0,1,0}).is_empty()){
            builder_block(hit_pos + glm::ivec3{0,1,0}, BlockType::EMPTY);
        }
    }
}

void Model::on_right_click() {
    Block result = player_hit_test(false);
    const auto& hit_pos {result.position};
    const auto& hit_tile_block {result.w};

    if(hit_pos.y > 0 && hit_pos.y < 256 && hit_tile_block.is_destructable()){
        if(!player->insersects_block(2, hit_pos)){
            // delete block
            builder_block(hit_pos, BlockType::EMPTY);
            //model_p->record_block(hit_pos);
        }
    }
}

void Model::on_middle_click() {
    const Block hit_block{player_hit_test(false)};
    const TileBlock& hit_tile_block {hit_block.w};
    if(hit_tile_block.is_user_buildable()){
        set_actual_item(hit_tile_block.getIndex());
    }
}

void Model::resolve_mouse_movement(const glm::vec2& offset) {
    float sensitivity = 0.1;
    player->rotate(offset * sensitivity);
}

double Model::resolve_scroll(double scroll_pos, double threshold) {
    if(scroll_pos < -threshold){
        set_prev_item();
        return 0;
    }

    if(scroll_pos > threshold){
        set_next_item();
        return 0;
    }
    return scroll_pos;
}

void Model::resolve_mouse_button(int button, int control, bool cursor_disabled) {
    switch(button){
        case GLFW_MOUSE_BUTTON_LEFT:
            if(cursor_disabled){
                control ? on_right_click() : on_left_click();
            }
            else{
                // restore camera control
                glfwSetInputMode(game_view.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            return;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if(cursor_disabled && !control) {on_right_click();}
            return;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            if(cursor_disabled) {on_middle_click();}
            return;
    }
}
