//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_MODEL_HPP
#define CPP_MODEL_HPP

#include <GLFW/glfw3.h>
#include <memory>
#include <unordered_map>

#include "Worker.hpp"
#include "Chunk.hpp"
#include "Player.hpp"
#include "costants.hpp"
#include "map_utils.hpp"

class Model {
    GLFWwindow* window;

    std::array<Worker, WORKERS> workers;
    std::unordered_map<glm::ivec2, Chunk> chunks;
public:
    static constexpr bool show_light = SHOW_LIGHT;
    const std::unordered_map<glm::ivec2, Chunk> &getChunks() const;

private:
    std::array<std::string_view, MAX_MESSAGES> messages;

    int create_radius;
    int render_radius;
    int delete_radius;
    int sign_radius;

    std::unique_ptr<Player> player{nullptr};
    int typing;

    char typing_buffer[MAX_TEXT_LENGTH];
    int message_index;

    int width;
    int height;

    int observe1;
    int observe2;
    bool flying;

    int item_index;
    int scale;
    bool ortho;

    float fov;
    int suppress_char;
    int mode;
    int mode_changed;
    std::string_view db_path;

    int day_length;
    int time_changed;

    std::array<Block,2> blocks;
    std::array<Block,2> blocks_copies;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

public:
    static constexpr int chunked(float val);
    static constexpr glm::ivec2 chunked(const glm::vec2& val);

    float get_day_time() const;
    float get_daylight() const;
    int get_scale_factor() const;
    bool is_flying() const;
    int getWidth() const;
    int getHeight() const;
    int getScale() const;
    void delete_player();
    void set_player(const glm::vec3& position, const glm::vec2& rotation, std::string_view name, int id);
    Chunk& get_chunk(const glm::ivec2& pq);
    static int get_chunk_distance(const glm::ivec2& pq1, const glm::ivec2& pq2);
    bool chunk_visible(const glm::ivec2& pq);
    Item highest_block(const glm::vec2& pq);
    bool has_light(const Chunk chunk);
};


#endif //CPP_MODEL_HPP
