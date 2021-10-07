//
// Created by ultimatenick on 07/10/21.
//

#include "DemoInteraction.hpp"

DemoInteraction::DemoInteraction(const GLFWwindow *window, FlyingCamera &flyingCamera,
                                 const glm::ivec2 &initialMousePos) : window{window}, flyingCamera{flyingCamera},
                                 mousePos{initialMousePos}{
    actualInstance = this;
}

DemoInteraction::~DemoInteraction() {
    actualInstance = nullptr;
}

void DemoInteraction::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    const glm::vec2 actualPos{xpos, ypos};
    const glm::vec2 offset{(actualInstance->mousePos - actualPos) * mouseSensitivity};
    actualInstance->mousePos = actualPos;

    actualInstance->flyingCamera.rotate(offset);
}
