//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_MODEL_HPP
#define CPP_MODEL_HPP

#define GLFW_INCLUDE_NONE

#include <unordered_map>
#include <memory>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Geometry/Chunk.hpp"
#include "Geometry/Sphere.hpp"
#include "glad/glad.h"
#include "costants.hpp"
#include "Rendering/Shader.hpp"
#include "Player.hpp"
#include "Geometry/Crosshair.hpp"
#include "Interaction/GameView.hpp"

class Model {
private:
    friend class ActionHandler;
    GameView& game_view;

    struct ShaderWrapper {
        const Shader &block_shader;
        const Shader &line_shader;
        const Shader &sky_shader;
        const Shader &text_shader;
    };

    std::unordered_map<unsigned , unsigned> textures{};
    std::unordered_map<glm::ivec2, Chunk> chunks{};
    ShaderWrapper shaders;
    const Sphere sky;

    std::unique_ptr<Player> player{nullptr};
    double previous_timestamp{};

    BlockType actual_item{TileBlock::items[0]};

    bool flying{false};

    int day_length;

    //std::array<Block, 2> blocks{};

    Crosshair crosshair;

    void load_collision_chunks();

    void load_visible_chunks();

    void remove_distant_chunks();

    void update_chunk_map();

    const Chunk & get_player_chunk() const;

    static Crosshair get_crosshair(int width, int height, int scale);

    glm::mat4 get_viewproj(GameView::proj_type pt) const;
public:
    Model(const Shader &block_shader, const Shader &line_shader, const Shader &sky_shader,
          const Shader &text_shader, GameView &game_view);

    float get_day_time() const;
    float get_daylight() const;
    void switch_flying();
    bool is_flying() const;
    void delete_player();
    void set_player(const glm::vec3 &position, const glm::vec2 &rotation, std::string_view name, int id);
    Chunk &get_chunk_at(const glm::ivec2 &pq);
    const Chunk &get_chunk_at(const glm::ivec2 &pq) const;
    int highest_block(const glm::vec2 &pq);
    void set_block(const glm::ivec3 &pos, BlockType w = BlockType::EMPTY);

    //void record_block(Block block);

    //void record_block(const glm::ivec3 &pos);

    TileBlock get_block(const glm::ivec3 position);

    void builder_block(const glm::ivec3 &pos, BlockType w);
    void render_chunks() const;
    void render_sky() const;
    static constexpr bool show_light = SHOW_LIGHT;
    const std::unordered_map<glm::ivec2, Chunk> &getChunks() const;
    void render_wireframe();
    void render_crosshair();
    void render_text(int justify, const glm::vec2 &position, float n, std::string_view text);
    void render_item();
    int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);

    //TileBlock get_actual_item() const;
    void set_actual_item(BlockType item_type);

    void set_next_item();

    void set_prev_item();

    Player *get_player() const;
    void set_perspective_properties(int fov, int orto);

    void handle_input(double dt);

    void render_scene();

    void swap_pool();

    void loop();

    GLFWwindow* get_window();

    std::array<const Chunk *, 6> chunk_neighbors_pointers(const glm::ivec2 &pq) const;
    Block player_hit_test(bool previous) const;
    std::pair<bool, glm::vec3> collide(int height, const std::unordered_map<glm::ivec2, Chunk> &chunk_map);

    void update_window();
};

#endif //CPP_MODEL_HPP
