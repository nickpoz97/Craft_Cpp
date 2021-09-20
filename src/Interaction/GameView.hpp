//
// Created by ultimatenick on 15/09/21.
//

#ifndef CPP_GAMEVIEW_HPP
#define CPP_GAMEVIEW_HPP

#define GLFW_INCLUDE_NONE

#include "mat4x4.hpp"
#include "GLFW/glfw3.h"
#include "../costants.hpp"

class GameView {
private:
    GLFWwindow *window{nullptr};

    int width;
    int height;
    int scale;
    int ortho;
    float fov;

    bool initialized = false;

    static constexpr float z_near = 0.125f;
    static constexpr float z_far = static_cast<float>(RENDER_CHUNK_RADIUS) * 32 + 64;

    glm::mat4 persp_proj{};
    glm::mat4 ortho_proj_2d{};
    glm::mat4 ortho_proj_3d{};
    glm::mat4 ortho_proj_item{};

    static int compute_scale_factor(int width, int height);

    GLFWwindow* create_window(bool is_fullscreen);
public:
    enum class proj_type {
        PERSP, UI, ORTHO_3D, ITEM
    };

    GameView(int width, int height, float fov, int ortho, bool is_fullscreen);
    ~GameView();

    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    [[nodiscard]] int get_scale() const;
    [[nodiscard]] int get_fov() const;
    [[nodiscard]] int get_ortho() const;
    [[nodiscard]] GLFWwindow* get_window() const;
    [[nodiscard]] float get_ratio() const;
    [[nodiscard]] bool is_initialized() const;

    void set_ortho(int ortho_size);
    void set_fov(int fov_degrees);
    void update();

    void update_all_proj_matrices();
    void update_persp_proj_matrix();
    void update_ortho_proj_matrix();

    glm::mat4 get_proj_matrix(proj_type pt) const;
};


#endif //CPP_GAMEVIEW_HPP
