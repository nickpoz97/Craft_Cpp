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
    glm::vec2 mousePos;
    const GLFWwindow* const window;
    FlyingCamera& flyingCamera;
public:
    DemoInteraction(FlyingCamera& flyingCamera);
};


#endif //CPP_DEMOINTERACTION_HPP
