//
// Created by ultimatenick on 13/08/21.
//

#include "Character.hpp"
#include "ext/matrix_transform.hpp"

Character::Character(const glm::vec2 &position, int m, int n, char c){
    int w = c - 32;

    float du = (w % 16) * a;
    float dv = 1 - (w / 16) * b - b;

    vertices = {{
        {position + glm::vec2{-n, -m}, glm::vec2{du, dv}},
        {position + glm::vec2{+n, -m}, glm::vec2{du + a, dv}},
        {position + glm::vec2{+n, +m}, glm::vec2{du + a, dv + b}},
        {position + glm::vec2{-n, -m}, glm::vec2{du, dv}},
        {position + glm::vec2{+n, +m}, glm::vec2{du + a, dv + b}},
        {position + glm::vec2{-n, +m}, glm::vec2{du, dv + b}}
    }};
}

