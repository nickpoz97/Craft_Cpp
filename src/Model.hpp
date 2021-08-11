//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_MODEL_HPP
#define CPP_MODEL_HPP

#include <GLFW/glfw3.h>
#include "Worker.hpp"
#include "Chunk.hpp"
#include "Player.hpp"

class Model {
    static constexpr int MAX_CHUNKS = 8192;
    static constexpr int WORKERS = 8192;
    static constexpr int MAX_TEXT_LENGTH = 256;
    static constexpr int MAX_MESSAGES = 256;

    GLFWwindow* window;
    std::array<Worker, WORKERS> workers;
    std::array<Chunk, MAX_CHUNKS> chunks;

    int chunk_count;
    int create_radius;
    int render_radius;
    int delete_radius;
    int sign_radius;

    Player player;
    int player_count;
    int typing;

    char typing_buffer[MAX_TEXT_LENGTH];
    int message_index;

    std::array<std::string_view, MAX_MESSAGES> messages;

    int width;
    int height;

    int observe1;
    int observe2;
    bool flying;

    int item_index;
    int scale;
    int ortho;

    float fov;
    int suppress_char;
    int mode;
    int mode_changed;
    std::string_view db_path;

    int day_length;
    int time_changed;

    std::array<Block,2> blocks;
    std::array<Block,2> blocks_copies;

public:
    float get_day_time() const;
    float get_daylight() const;
    int get_scale_factor() const;
    bool is_flying() const;

    int getWidth() const;

    int getHeight() const;

    int getScale() const;
};


#endif //CPP_MODEL_HPP
