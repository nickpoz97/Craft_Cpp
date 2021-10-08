//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_CAMERACONTROL_HPP
#define CPP_CAMERACONTROL_HPP

#include <memory>
#include <vec2.hpp>
#include "GLFW/glfw3.h"
#include "FlyingCamera.hpp"
#include "GameView.hpp"

class CameraControl {
private:
    static inline std::unique_ptr<CameraControl> actualInstance{nullptr};
    glm::vec2 mousePos;
    FlyingCamera& flyingCamera;
    static constexpr float mouseSensitivity{0.1};
    double lastFrameTime{};
    CameraControl(FlyingCamera& flyingCamera, const glm::ivec2& initialMousePos);
public:
    static CameraControl * setInstance(FlyingCamera& flyingCamera);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void processKeyboardInput() ;
};


#endif //CPP_CAMERACONTROL_HPP
