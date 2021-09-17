//
// Created by ultimatenick on 10/08/21.
//
#define GLFW_INCLUDE_NONE

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"
#include "Player.hpp"
#include "Model.hpp"
#include "Sphere.hpp"
#include "CubeWireframe.hpp"
#include "Text2D.hpp"
#include "Item.hpp"
#include "stb_image.h"
#include "ActionHandler.hpp"
#include "fmt/format.h"
#include "Shader.hpp"
#include "Chunk.hpp"
#include "GameView.hpp"

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

        if(c.is_visible(get_viewproj(pt)) && get_chunk_distance(get_player_chunk(), c) < RENDER_CHUNK_RADIUS){
            c.render_object();
        }
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

    const glm::mat4 view{
            glm::lookAt(player->get_position(),
            player->get_position() + player->get_camera_direction_vector(),
            {0, 1, 0})
    };

    return view * game_view.get_proj_matrix(pt);
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
    shader.set_viewproj(game_view.get_proj_matrix(GameView::proj_type::TEXT));
    crosshair.render_lines();
}

void Model::render_text(int justify, const glm::vec2 &position, float n, std::string_view text) {
    const Shader& shader = shaders.text_shader;

    shader.use();
    shader.set_viewproj(game_view.get_proj_matrix(GameView::proj_type::TEXT));
    shader.set_sampler(1);
    const glm::vec2 justified_position{position - glm::vec2{n * justify * (text.size() - 1) / 2, 0}};
    Text2D{justified_position, n, text}.render_object();
}

void Model::render_item() {
    const Shader& shader = shaders.block_shader;
    shader.use();
    // TODO use another type of matrix
    shader.set_viewproj(get_viewproj(GameView::proj_type::TEXT));
    shader.set_camera({0,0,5});
    shader.set_sampler(0);
    shader.set_timer(get_day_time());

    Item item_geometry{actual_item};
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
    ActionHandler::set_callbacks(game_view.get_window());
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

        std::string_view s{
            fmt::format("(%d, %d) (%.2f, %.2f, %.2f) n_chunks: %d, hour: %d%cm",
                        p, q, x, y, z, chunks.size(), hour, am_pm)
        };

        render_text(ALIGN_LEFT, {tx, ty}, ts, s);
    }
}

bool Model::swap_pool() {
    glfwSwapBuffers(game_view.get_window());
    glfwPollEvents();
    return !glfwWindowShouldClose(game_view.get_window());
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

bool Model::loop() {
    game_view.update_window();
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

    const glm::ivec3 int_pos{glm::abs(position)};
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

Crosshair Model::get_crosshair(int width, int height, int scale) {
    return Crosshair(width, height, scale);
}

void Model::set_perspective_properties(int fov, int orto) {
    game_view.set_fov(fov);
    game_view.set_ortho(orto);
}
