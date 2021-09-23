//
// Created by ultimatenick on 04/09/21.
//

#ifndef CPP_ACTIONHANDLER_HPP
#define CPP_ACTIONHANDLER_HPP

#define GLFW_INCLUDE_NONE

#include "Model.hpp"
#include "Player.hpp"
#include "GLFW/glfw3.h"

class ActionHandler {
private:
    static inline Model* model_p{nullptr};

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void scroll_callback(GLFWwindow *window, double xdelta, double ydelta);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    static void set_callbacks();
public:
    static void initialize(Model* model_address);
    static void destroy();
    static void handle_key_pressing(double delta_t);
    static void handle_mouse_position();
    static bool is_enabled_for(Model* m_ref);
};


#endif //CPP_ACTIONHANDLER_HPP
