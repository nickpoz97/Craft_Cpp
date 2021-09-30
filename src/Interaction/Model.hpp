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
#include "../Geometry/Chunk.hpp"
#include "../Geometry/Sphere.hpp"
#include "glad/glad.h"
#include "../costants.hpp"
#include "../Rendering/Shader.hpp"
#include "Player.hpp"
#include "../Geometry/Crosshair.hpp"
#include "GameView.hpp"

enum class ShaderName{
    BLOCK_SHADER,
    LINE_SHADER,
    SKY_SHADER,
    TEXT_SHADER
};

struct ShaderFilesPaths{
    std::string_view vertex_code;
    std::string_view fragment_code;
};

struct GameViewSettings{
    int width;
    int height;
    float fov;
    int ortho;
    bool is_fullscreen;
};

using ShaderNamesMap =  std::unordered_map<ShaderName, ShaderFilesPaths>;

class Model {
private:
    friend class ActionHandler;
    GameView game_view;
    std::unordered_map<ShaderName, const Shader> shaders;

    std::unordered_map<glm::ivec2, Chunk> chunks{};
    const Sphere sky;
    Crosshair crosshair;
    Player player;

    double previous_timestamp{};

    BlockType actual_item{TileBlock::items[0]};

    bool flying{false};

    int day_length;

    //std::array<Block, 2> blocks{};

    void load_chunks_in_range();
    void remove_distant_chunks();
    void update_chunk_map();

    const Chunk & get_player_chunk() const;

    glm::mat4 get_viewproj(GameView::ProjType pt) const;

    void on_left_click();
    void on_right_click();
    void on_middle_click();
public:
    Model(const ShaderNamesMap & shaders_names_map, const GameViewSettings & gvs);

    float get_day_time() const;
    float get_daylight() const;
    void switch_flying();
    bool is_flying() const;

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
    static int load_texture(std::string_view path, GLint clamp_type = GL_REPEAT);

    //TileBlock get_actual_item() const;
    void set_actual_item(BlockType item_type);
    void set_next_item();
    void set_prev_item();

    void set_perspective_properties(int fov, int orto);

    void handle_input(double dt);
    void render_scene();
    void swap_pool();
    void loop();

    GLFWwindow* get_window();

    std::array<const Chunk *, 6> chunk_neighbors_pointers(const glm::ivec2 &pq) const;
    Block player_hit_test(bool previous) const;

    void resolve_mouse_movement(const glm::vec2& offset);
    double resolve_scroll(double scroll_pos, double threshold);
};

#endif //CPP_MODEL_HPP
