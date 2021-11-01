//
// Created by ultimatenick on 07/10/21.
//

#include <iostream>
#include "Interaction/CameraControl.hpp"
#include "GLFW/glfw3.h"

namespace CraftCpp {
CameraControl::CameraControl(Camera &flyingCamera, const glm::ivec2 &initialMousePos, GLFWwindow *window)
        : flyingCamera{flyingCamera},
          lastMousePos{initialMousePos} {
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(GameView::getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cursorInvisible = true;
#ifndef NDEBUG
    glfwSetInputMode(GameView::getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cursorInvisible = false;
#endif
}

void CameraControl::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    if (!actualInstance) {
        std::cerr << "CameraControl not instantiated, mouseCallback not available\n";
        return;
    }
    static bool firstMouse{true};
    if (firstMouse) {
        actualInstance->lastMousePos.x = xpos;
        actualInstance->lastMousePos.y = ypos;
        firstMouse = false;
        return;
    }

    glm::vec2 offset{
            xpos - actualInstance->lastMousePos.x,
            actualInstance->lastMousePos.y - ypos
    };
    offset *= mouseSensitivity;

    actualInstance->lastMousePos = {xpos, ypos};

    actualInstance->flyingCamera.rotate(offset);
}

void CameraControl::processKeyboardInput() {
    if (!GameView::isInstantiated()) {
        std::cerr << "GameView not instantiated, input from keyboard does nothing\n";
        return;
    }
    double actualFrameTime{glfwGetTime()};

    float deltaTime = static_cast<bool>(lastFrameTime) ? static_cast<float>(actualFrameTime - lastFrameTime) : 0;
    lastFrameTime = actualFrameTime;

    GLFWwindow *window{GameView::getWindow()};

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        flyingCamera.shiftFront(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        flyingCamera.shiftFront(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        flyingCamera.shiftRight(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        flyingCamera.shiftRight(deltaTime);
}

std::unique_ptr<CameraControl> CameraControl::setInstance(Camera &flyingCamera) {
    if (!actualInstance && GameView::isInstantiated()) {
        glm::vec2 initialMousePos{
                GameView::getActualInstance()->get_width() / 2,
                GameView::getActualInstance()->get_height() / 2
        };
        actualInstance = new CameraControl{flyingCamera, initialMousePos, GameView::getWindow()};

        auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods){
            if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
                actualInstance->switchCursorStatus();
            }
        };

        glfwSetKeyCallback(GameView::getWindow(), keyCallback);
    }
    return std::unique_ptr<CameraControl>{actualInstance};
}

    void CameraControl::switchCursorStatus(){
        cursorInvisible ?
            (glfwSetInputMode(GameView::getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL), cursorInvisible = false) :
            (glfwSetInputMode(GameView::getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED), cursorInvisible = true);
    }

    CameraControl::~CameraControl() {
        actualInstance = nullptr;
    }
}