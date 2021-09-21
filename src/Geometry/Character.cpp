//
// Created by ultimatenick on 13/08/21.
//

#include "Character.hpp"
#include "ext/matrix_transform.hpp"

Character::Character(const glm::vec2 &position, float m, float n, char c){
    int w = c - 32;

    float du = static_cast<float>(w % 16) * a;
    float dv = 1 - static_cast<float>(w / 16) * b - b;

    vertices = {{
        {position + glm::vec2{-n, -m}, glm::vec2{du, dv}},
        {position + glm::vec2{+n, -m}, glm::vec2{du + a, dv}},
        {position + glm::vec2{+n, +m}, glm::vec2{du + a, dv + b}},
        {position + glm::vec2{-n, -m}, glm::vec2{du, dv}},
        {position + glm::vec2{+n, +m}, glm::vec2{du + a, dv + b}},
        {position + glm::vec2{-n, +m}, glm::vec2{du, dv + b}}
    }};
}

int Character::get_n_vertices() {
    return n_vertices;
}

