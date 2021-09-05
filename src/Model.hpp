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

using Block = std::pair<glm::ivec3, TileBlock>;

class Model {
    struct ShaderWrapper{
        Shader block_shader;
        Shader line_shader;
        Shader sky_shader;
    };
private:

    GLFWwindow* window;
    std::array<Worker, WORKERS> workers;
    std::unordered_map<glm::ivec2, Chunk> chunks;
    ShaderWrapper shaders;
    //std::array<std::string_view, MAX_MESSAGES> messages;
    static const Sphere sky;

    int create_radius;
    static constexpr int render_radius = RENDER_CHUNK_RADIUS;

    static constexpr float z_near = 0.125f;
    static constexpr float z_far = static_cast<float>(render_radius) * 32 + 64
    int delete_radius;
    int sign_radius;

    std::unique_ptr<Player> player{nullptr};
    //int typing;

    char typing_buffer[MAX_TEXT_LENGTH];
    int message_index;

    int width;
    int height;

    int observe1;
    int observe2;

    TileBlock actual_item;
    int scale;
    int ortho;

    float fov;

    int suppress_char;
    int mode;
    int mode_changed;
    //std::string_view db_path;

    int day_length;
    int time_changed;

    std::array<Block,2> blocks;
    std::array<Block,2> blocks_copies;

    enum class proj_type{PERSP, ORTHO_2D};
    const glm::mat4 persp_proj{glm::perspective(glm::radians(fov), static_cast<float>(width) / (height), z_near, z_far)};
    static const glm::mat4 ortho_proj_2d{glm::ortho(0, width, 0, height, -1, 1)};

    Crosshair crosshair{*this};
    glm::mat4 get_viewproj(proj_type pt);
    void create_window(bool is_fullscreen);
public:
    Model();
    ~Model();

    static constexpr glm::ivec2 chunked(const glm::vec3& position);
    static constexpr int chunked(int val);

    float get_day_time() const;
    float get_daylight() const;
    int get_scale_factor() const;
    void switch_flying();
    int getWidth() const;
    int getHeight() const;
    int getScale() const;
    void delete_player();
    void set_player(const glm::vec3& position, const glm::vec2& rotation, std::string_view name, int id);
    Chunk& get_chunk_at(const glm::ivec2& pq);
    static int get_chunk_distance(const glm::ivec2& pq1, const glm::ivec2& pq2);
    bool chunk_visible(const glm::ivec2& pq);
    TileBlock highest_block(const glm::vec2& pq);

    void set_block(const glm::ivec3& pos, const TileBlock& w);
    void set_block(const glm::ivec3& pos);

    void record_block(Block block);
    void record_block(const glm::ivec3& pos, const TileBlock& w);
    void record_block(const glm::ivec3& pos);

    TileBlock get_block(const glm::ivec3 position);
    void builder_block(const glm::ivec3 &pos, BlockType w);
    void render_chunks();
    void render_sky() const;
    int getRenderRadius() const;
    float getFov() const;
    static constexpr bool show_light = SHOW_LIGHT;
    const std::unordered_map<glm::ivec2, Chunk> &getChunks() const;
    void render_wireframe();
    void render_crosshair();
    void render_text(int justify, const glm::vec3 &position, int n, std::string_view text);
    void render_item();

    TileBlock get_actual_item() const;
    void set_actual_item(TileBlock item);
    void set_next_item();
    void set_prev_item();

    GLFWwindow* get_window();

    void set_ortho(int ortho_size) const;
    void set_fov(int fov_degrees) const;
    Player* get_player() const;
};


#endif //CPP_MODEL_HPP
