//
// Created by ultimatenick on 10/08/21.
//

#include <cmath>
#include <glad/glad.h>
#include <gtc/matrix_transform.hpp>

#include "Model.hpp"
#include "Player.hpp"
#include "trigonometric.hpp"

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

    int miny = Chunk::getMinY();
    int maxy = Chunk::getMaxY();

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

TileBlock Model::highest_block(const glm::vec2& pq) {
    const Chunk& chunk = chunks.at(pq);
    return chunk.getHighestBlock();
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
    auto c_it = chunks.find(pq);
    return (c_it != chunks.end()) ? c_it->second : chunks.emplace(pq, Chunk{*this,pq});
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
    if (pos.y <= 0 || pos.y >= 256 || w == BlockType::EMPTY) {
        return;
    }
    set_block(pos, TileBlock{});
    /*if (get_block(pos).is_destructable()) {
        set_block(pos, TileBlock{});
    }*/
    /*if (w != BlockType::EMPTY) {
        set_block(pos, TileBlock{w});
    }*/
}

glm::ivec2 Model::get_player_pq() {
    if(player){
        return chunked(player->get_position());
    }
    return {};
}

void Model::render_chunks() {
    if(!player || !shaders.block_shader.get_id()){
        return;
    }

    const Shader& shader = shaders.block_shader;
    shader.use();

    glm::ivec2 player_pq = get_player_pq();
    int light = get_daylight();
    glm::mat4 viewproj{
        glm::perspective(glm::radians(fov), static_cast<float>(width)/(height), z_near, z_far) *
        glm::lookAt(player->get_position(), player->get_position() + player->get_camera_direction_vector(), {0,1,0})
    };

    shader.set_viewproj(viewproj);
    shader.set_camera(player->get_position());
    shader.set_sampler(0);
    shader.set_timer(get_day_time());
    shader.set_extra(1, 2);
    shader.set_extra(2, light);
    shader.set_extra(3, render_radius * Chunk::size);
    shader.set_extra(4, ortho);

    for(const auto& c : chunks){
        if(c.is_visible(player->getFrustum()) && get_chunk_distance(player_pq, c.pq) < render_radius){
            c.render();
        }
    }
}

int Model::getRenderRadius() const {
    return render_radius;
}

float Model::getFov() const {
    return fov;
}
