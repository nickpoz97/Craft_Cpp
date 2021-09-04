//
// Created by ultimatenick on 04/09/21.
//

#ifndef CPP_ACTIONHANDLER_HPP
#define CPP_ACTIONHANDLER_HPP


#include "Player.hpp"

// TODO check if everything works (semi singleton used)
class ActionHandler {
    const Player& player;
    Model &model;
    static inline std::shared_ptr<const ActionHandler> instance{nullptr};

    //void on_light();

    void on_right_click() const;
    void on_left_click() const;
    void on_middle_click() const;

    static void on_key(GLFWwindow *window, int key, int scancode, int action, int mods);
public:
    static std::shared_ptr<const ActionHandler> get_instance();
    ActionHandler(const Player &player, Model &model);
};


#endif //CPP_ACTIONHANDLER_HPP
