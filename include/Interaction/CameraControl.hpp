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

class CameraControl {
private:
    static inline std::unique_ptr<CameraControl> actualInstance{nullptr};
    glm::vec2 lastMousePos;
    Camera& flyingCamera;
    static constexpr float mouseSensitivity{0.1};
    double lastFrameTime{};
    CameraControl(Camera &flyingCamera, const glm::ivec2 &initialMousePos, GLFWwindow *window);
public:
    static CameraControl * setInstance(Camera& flyingCamera);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void processKeyboardInput() ;
};


#endif //CPP_CAMERACONTROL_HPP
