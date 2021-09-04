//
// Created by ultimatenick on 04/09/21.
//

#ifndef CPP_ACTIONHANDLER_HPP
#define CPP_ACTIONHANDLER_HPP


#include "Player.hpp"

// TODO check if everything works (semi singleton used)
class ActionHandler {
    static inline Player* player_p{nullptr};
    static inline Model* model_p{nullptr};
    static inline bool initialized{false};

    //void on_light();

    static void on_right_click();
    static void on_left_click();
    static void on_middle_click();
    static void on_key(GLFWwindow *window, int key, int scancode, int action, int mods);
public:
    static void attach_components(Player* player_address, Model* model_address);
};


#endif //CPP_ACTIONHANDLER_HPP
