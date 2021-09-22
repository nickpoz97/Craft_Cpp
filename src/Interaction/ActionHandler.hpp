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
    static inline Player* player_p{nullptr};
    static inline Model* model_p{nullptr};
    static inline GameView* game_view_p{nullptr};

    static inline bool initialized{false};
    static inline bool members_set{false};

    static void on_right_click();
    static void on_left_click();
    static void on_middle_click();
    static glm::vec3 compute_motion(double delta_t);

    static void on_key(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void on_scroll(GLFWwindow *window, double xdelta, double ydelta);
    static void on_mouse_button(GLFWwindow *window, int button, int action, int mods);

public:
    static void initialize(Model* model_address);
    static void set_callbacks(GLFWwindow* window);
    static void handle_movement(double delta_t);
    static void handle_mouse_input();
};


#endif //CPP_ACTIONHANDLER_HPP