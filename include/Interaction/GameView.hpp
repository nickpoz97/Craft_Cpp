//
// Created by ultimatenick on 15/09/21.
//

#ifndef CPP_GAMEVIEW_HPP
#define CPP_GAMEVIEW_HPP

#define GLFW_INCLUDE_NONE

#include <memory>
#include <Geometry/Chunk.hpp>

#include "Rendering/OpenGlExternal.hpp"
#include "glm/mat4x4.hpp"
#include "tunableParameters.hpp"

namespace CraftCpp {
class GameView {
private:
    static inline GameView* actualInstance{nullptr};
    GLFWwindow *window;

    int width;
    int height;
    int scale;
    int ortho;
    float fov;

    static constexpr float z_near = 0.125f;
    static constexpr float z_far = static_cast<float>(RENDER_CHUNK_RADIUS) * Chunk::SIZE;

    static int computeScaleFactor(int width, int height);

    GLFWwindow *createWindow(bool is_fullscreen);

    GameView(int width, int height, float fov, int ortho = false, bool isFullscreen = false);

public:
    enum ProjType {
        PERSP, UI, ORTHO_3D, ITEM
    };

    static std::unique_ptr<GameView> setInstance(int width, int height, float fov, int ortho = false, bool isFullscreen = false);

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] int getScale() const;

    [[nodiscard]] int getFov() const;

    [[nodiscard]] int getOrtho() const;

    [[nodiscard]] static GLFWwindow *getWindow();

    [[nodiscard]] float getRatio() const;

    [[nodiscard]] static bool isInstantiated();

    [[nodiscard]] float item_box_side() const;

    [[nodiscard]] static GameView *getActualInstance();

    void setOrtho(int ortho_size);

    void setFov(int fov_degrees);

    void update();

    [[nodiscard]] glm::mat4 getProjMatrix(ProjType pt) const;

    ~GameView();
};
}

#endif //CPP_GAMEVIEW_HPP
