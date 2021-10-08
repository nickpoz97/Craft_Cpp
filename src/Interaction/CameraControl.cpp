//
// Created by ultimatenick on 07/10/21.
//

#include "CameraControl.hpp"
#include "GLFW/glfw3.h"

CameraControl::CameraControl(FlyingCamera &flyingCamera,
                             const glm::ivec2 &initialMousePos) : flyingCamera{flyingCamera},
                                 mousePos{initialMousePos}{
    glfwSetCursorPosCallback(GameView::getActualInstance()->getWindow(), mouseCallback);
}

void CameraControl::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    const glm::vec2 actualPos{xpos, ypos};
    const glm::vec2 offset{(actualInstance->mousePos - actualPos) * mouseSensitivity};
    actualInstance->mousePos = actualPos;

    actualInstance->flyingCamera.rotate(offset);
}

void CameraControl::processKeyboardInput() {
    if(!GameView::isInstantiated()){
        return;
    }
    double actualFrameTime{glfwGetTime()};

    float deltaTime = static_cast<bool>(lastFrameTime) ? static_cast<float>(actualFrameTime - lastFrameTime) : 0;
    lastFrameTime = actualFrameTime;

    GLFWwindow* window{GameView::getActualInstance()->getWindow()};

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        flyingCamera.shiftFront(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        flyingCamera.shiftFront(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        flyingCamera.shiftRight(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        flyingCamera.shiftRight(deltaTime);
}

CameraControl * CameraControl::setInstance(FlyingCamera &flyingCamera) {
    if(!actualInstance) {
        glm::vec2 initialMousePos{
                GameView::getActualInstance()->get_width()/2,
                GameView::getActualInstance()->get_height()/2
        };
        actualInstance.reset(new CameraControl{flyingCamera, initialMousePos});
    }
    return actualInstance.get();
}
