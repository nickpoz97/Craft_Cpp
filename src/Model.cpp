//
// Created by ultimatenick on 10/08/21.
//

#include <cmath>

#include "Model.hpp"
#include "Player.hpp"

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

Chunk &Model::get_chunk(const glm::ivec2& pq) {
    return chunks.at(pq);
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

constexpr int Model::chunked(float val) {
    return glm::floor(glm::round(val) / Chunk::getSize());
}

Item Model::highest_block(const glm::vec2& pq) {
    const Chunk& chunk = chunks.at(pq);
    return chunk.getHighestBlock();
}

constexpr glm::ivec2 Model::chunked(const glm::vec2& val) {
    return {chunked(val.x), chunked(val.y)};
}

const std::unordered_map<glm::ivec2, Chunk> &Model::getChunks() const {
    return chunks;
}

int Model::get_chunk_distance(const glm::ivec2 &pq1, const glm::ivec2 &pq2) {
        auto delta = pq1 - pq2;
        return glm::max(glm::abs(delta.x), glm::abs(delta.y));
}

bool Model::has_light(const Chunk chunk) {
    return false;
}

