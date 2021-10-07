//
// Created by ultimatenick on 07/10/21.
//

#ifndef CPP_DEMOINTERACTION_HPP
#define CPP_DEMOINTERACTION_HPP


#include <vec2.hpp>
#include "GLFW/glfw3.h"
#include "FlyingCamera.hpp"

class DemoInteraction {
private:
    static inline DemoInteraction* actualInstance{nullptr};
    glm::vec2 mousePos;
    const GLFWwindow* const window;
    FlyingCamera& flyingCamera;
    static constexpr float mouseSensitivity{0.1};
public:
    DemoInteraction(const GLFWwindow* window, FlyingCamera& flyingCamera, const glm::ivec2& initialMousePos);
    ~DemoInteraction();
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};


#endif //CPP_DEMOINTERACTION_HPP
