//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_MODEL_HPP
#define CPP_MODEL_HPP

#include <GLFW/glfw3.h>
#include <memory>
#include <unordered_map>

#include "Chunk.hpp"
#include "Player.hpp"
#include "costants.hpp"
#include "map_utils.hpp"

using Block = std::pair<glm::ivec3, TileBlock>;

class Model {
    struct ShaderWrapper{
        const Shader& block_shader{
            "block_vertex.vs", "block_fragment.fs", {"sky_sampler", "daylight", "fog_distance", "ortho"}
        };
        const Shader& line_shader{
            "line_vertex.vs", "line_fragment.fs"
        };
        const Shader& sky_shader{
            "sky_vertex.vs", "sky_fragment.fs"
        };
        const Shader& text_shader{
            "text_vertex.vs", "text_fragment.fs"
        };
    };
private:

    GLFWwindow* window;
    std::unordered_map<glm::ivec2, Chunk> chunks{};
    ShaderWrapper shaders;
    static const Sphere sky;

    std::unique_ptr<Player> player{nullptr};

    int width;
    int height;
    double previous_timestamp{};

    TileBlock& actual_item{TileBlock::items[0]};
    int scale;
    int ortho;

    float fov;
    bool flying{false};

    int day_length;

    std::array<Block,2> blocks{};

    enum class proj_type{PERSP, ORTHO_2D};
    glm::mat4 persp_proj{glm::perspective(glm::radians(fov), static_cast<float>(width) / (height), z_near, z_far)};
    glm::mat4 ortho_proj_2d{glm::ortho(0, width, 0, height, -1, 1)};

    Crosshair crosshair{*this};
    glm::mat4 get_viewproj(proj_type pt) const;
    GLFWwindow * create_window(bool is_fullscreen);

    void update_proj_matrix();

    void load_collision_chunks();
    void load_visible_chunks();
    void remove_distant_chunks();
    void update_chunk_map();
public:
    static constexpr float z_near = 0.125f;
    static constexpr float z_far = static_cast<float>(RENDER_CHUNK_RADIUS) * 32 + 64;

    Model();
    Model(const Shader &block_shader, const Shader &line_shader, const Shader &sky_shader,
          const Shader &text_shader);
    ~Model();

    static constexpr glm::ivec2 chunked(const glm::vec3& position);
    static constexpr int chunked(int val);

    float get_day_time() const;
    float get_daylight() const;
    int get_scale_factor() const;
    void switch_flying();
    bool is_flying() const;
    int getWidth() const;
    int getHeight() const;
    int getScale() const;
    void delete_player();
    void set_player(const glm::vec3& position, const glm::vec2& rotation, std::string_view name, int id);
    Chunk& get_chunk_at(const glm::ivec2& pq);
    static int get_chunk_distance(const glm::ivec2& pq1, const glm::ivec2& pq2);
    bool chunk_visible(const glm::ivec2& pq);

    int highest_block(const glm::vec2& pq);

    void set_block(const glm::ivec3& pos, const TileBlock& w);
    void set_block(const glm::ivec3& pos);

    void record_block(Block block);
    void record_block(const glm::ivec3& pos, const TileBlock& w);
    void record_block(const glm::ivec3& pos);

    TileBlock get_block(const glm::ivec3 position);
    void builder_block(const glm::ivec3 &pos, BlockType w);
    void render_chunks() const;
    void render_sky() const;
    float getFov() const;
    static constexpr bool show_light = SHOW_LIGHT;
    const std::unordered_map<glm::ivec2, Chunk> &getChunks() const;
    void render_wireframe();
    void render_crosshair();
    void render_text(int justify, const glm::vec2 &position, float n, std::string_view text);
    void render_item();

    //TileBlock get_actual_item() const;
    void set_actual_item(TileBlock item);
    void set_next_item();
    void set_prev_item();

    GLFWwindow* get_window();

    void set_ortho(int ortho_size) ;
    void set_fov(int fov_degrees) ;
    Player* get_player() const;

    void update_window_size();
    void handle_input(double dt);

    void render_scene();
    bool swap_pool();

    bool loop();
};


#endif //CPP_MODEL_HPP
