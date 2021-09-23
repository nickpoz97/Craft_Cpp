//
// Created by ultimatenick on 04/09/21.
//

#define GLFW_INCLUDE_NONE

#include <iostream>
#include "trigonometric.hpp"
#include "ActionHandler.hpp"
#include "Model.hpp"
#include "GLFW/glfw3.h"
#include "Player.hpp"

void ActionHandler::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // no action handler check
    if(action != GLFW_PRESS){
        return;
    }

    int control = mods & (GLFW_MOD_CONTROL | GLFW_MOD_SUPER);
    // indicates if mouse is used for camera motion
    bool cursor_disabled = (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);

    model_p->resolve_keyboard_input(key, control, cursor_disabled);
}

void ActionHandler::initialize(Model *model_address){
    if(!model_p) {
        bool initialized = model_address && model_address->get_window() && model_address->get_player();
        if (initialized) {
            model_p = model_address;
#ifdef DEBUG
            glfwSetInputMode(model_p->get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif
            set_callbacks();
        }
    }
}

void ActionHandler::scroll_callback(GLFWwindow *window, double xdelta, double ydelta) {
    static double former_scroll_pos{};
    former_scroll_pos = model_p->resolve_scroll(former_scroll_pos + ydelta, SCROLL_TRESHOLD);
}

void ActionHandler::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if(action != GLFW_PRESS){
        return;
    }

    int control = mods & (GLFW_MOD_CONTROL | GLFW_MOD_SUPER);
    bool cursor_disabled = (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);

    model_p->resolve_mouse_button(button, control, cursor_disabled);
}

void ActionHandler::handle_key_pressing(double delta_t) {
    int ortho = (glfwGetKey(model_p->get_window(), 'F') ? 64 : 0);
    int fov = (glfwGetKey(model_p->get_window(), GLFW_KEY_LEFT_SHIFT) ? 15 : 65);
    model_p->set_perspective_properties(fov, ortho);

    int x_movement{}, z_movement{};
    auto* player_p = model_p->get_player();

    if (glfwGetKey(model_p->get_window(), 'W')) { z_movement--; }
    if (glfwGetKey(model_p->get_window(), 'S')) { z_movement++; }
    if (glfwGetKey(model_p->get_window(), 'A')) { x_movement--; }
    if (glfwGetKey(model_p->get_window(), 'D')) { x_movement++; }
    if (glfwGetKey(model_p->get_window(), GLFW_KEY_LEFT)) player_p->rotate({-delta_t, 0});
    if (glfwGetKey(model_p->get_window(), GLFW_KEY_RIGHT)) player_p->rotate({delta_t, 0});
    if (glfwGetKey(model_p->get_window(), GLFW_KEY_UP)) player_p->rotate({0, delta_t});
    if (glfwGetKey(model_p->get_window(), GLFW_KEY_DOWN)) player_p->rotate({0, -delta_t});
    bool jump_action = glfwGetKey(model_p->get_window(), GLFW_KEY_SPACE);

    player_p->apply_movement(delta_t, model_p->is_flying(), jump_action, {x_movement, z_movement}, model_p->getChunks());
}

void ActionHandler::handle_mouse_position() {
    static glm::vec2 former_cursor_pos{};

    bool cursor_disabled = glfwGetInputMode(model_p->get_window(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;

    if(cursor_disabled && former_cursor_pos != glm::vec2{}){
        double mx, my;
        glfwGetCursorPos(model_p->get_window(), &mx, &my);
        glm::vec2 actual_cursor_pos{mx, my};
        model_p->resolve_mouse_movement(actual_cursor_pos - former_cursor_pos);
        former_cursor_pos = actual_cursor_pos;
    }
}

void ActionHandler::set_callbacks() {
    auto* window = model_p->get_window();

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height){
        model_p->update_window();
    };
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void ActionHandler::destroy() {
    model_p = nullptr;
}

bool ActionHandler::is_enabled_for(Model *m_ref) {
    return model_p == m_ref;
}
