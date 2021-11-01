//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_CAMERACONTROL_HPP
#define CPP_CAMERACONTROL_HPP

#include <memory>
#include <glm/vec2.hpp>

#include "glfw/include/GLFW/glfw3.h"
#include "Interaction//Camera.hpp"
#include "Interaction/GameView.hpp"

namespace CraftCpp {
class CameraControl {
private:
    static inline CameraControl* actualInstance{nullptr};
    glm::vec2 lastMousePos;
    Camera &flyingCamera;
    static constexpr float mouseSensitivity{0.1};
    double lastFrameTime{};

    bool cursorInvisible{true};
    void switchCursorStatus();

    CameraControl(Camera &flyingCamera, const glm::ivec2 &initialMousePos, GLFWwindow *window);

public:
    static std::unique_ptr<CameraControl> setInstance(Camera &flyingCamera);
    ~CameraControl();

    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);

    void processKeyboardInput();
};
}

#endif //CPP_CAMERACONTROL_HPP
