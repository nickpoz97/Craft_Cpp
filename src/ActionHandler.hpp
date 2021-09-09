//
// Created by ultimatenick on 04/09/21.
//

#ifndef CPP_ACTIONHANDLER_HPP
#define CPP_ACTIONHANDLER_HPP


#include "Player.hpp"

// TODO check if everything works (semi singleton used)
class ActionHandler {
private:
    static inline Player* player_p{nullptr};
    static inline Model* model_p{nullptr};

    static inline bool initialized{false};
    static inline bool members_set{false};

    static inline double scroll_pos = 0;
    static inline double delta_y = 0;

    static inline glm::vec2 former_cursor_pos{};

    static void on_right_click();
    static void on_left_click();
    static void on_middle_click();
    static glm::vec3 compute_motion(double delta_t, int x_movement, int z_movement);

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
