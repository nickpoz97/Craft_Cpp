//
// Created by ultimatenick on 15/09/21.
//

#ifndef CPP_GAMEVIEW_HPP
#define CPP_GAMEVIEW_HPP

#define GLFW_INCLUDE_NONE

#include <memory>
#include "mat4x4.hpp"
#include "GLFW/glfw3.h"
#include "../costants.hpp"

class GameView {
private:
    static inline std::unique_ptr<GameView> actualInstance{nullptr};
    GLFWwindow *window;

    int width;
    int height;
    int scale;
    int ortho;
    float fov;

    static constexpr float z_near = 0.125f;
    static constexpr float z_far = static_cast<float>(RENDER_CHUNK_RADIUS) * 32 + 64;

    static int compute_scale_factor(int width, int height);

    GLFWwindow* create_window(bool is_fullscreen);
    GameView(int width, int height, float fov, int ortho = false, bool is_fullscreen = false);
public:
    enum ProjType {
        PERSP, UI, ORTHO_3D, ITEM
    };
    static GameView* setInstance(int width, int height, float fov, int ortho = false, bool is_fullscreen = false);

    [[nodiscard]] int get_width() const;
    [[nodiscard]] int get_height() const;
    [[nodiscard]] int get_scale() const;
    [[nodiscard]] int get_fov() const;
    [[nodiscard]] int get_ortho() const;
    [[nodiscard]] GLFWwindow* getWindow();
    [[nodiscard]] float get_ratio() const;
    [[nodiscard]] static bool isInstantiated();
    [[nodiscard]] float item_box_side() const;
    [[nodiscard]] static GameView* getActualInstance();

    void set_ortho(int ortho_size);
    void set_fov(int fov_degrees);
    void update();

    [[nodiscard]] glm::mat4 get_proj_matrix(ProjType pt) const;

    static void freeGLFWResources();
};


#endif //CPP_GAMEVIEW_HPP
