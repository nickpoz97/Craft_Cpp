//
// Created by ultimatenick on 13/08/21.
//

#include "Character.hpp"
#include "ext/matrix_transform.hpp"

Character::Character(const glm::vec2 &position, int m, int n, char c) : GameObject<Uv3DVertex, 6>(){
    int w = c - 32;

    float du = (w % 16) * a;
    float dv = 1 - (w / 16) * b - b;

    vertices = {{
        {glm::vec2{-n, -m}, glm::vec2{du, dv}},
        {glm::vec2{+n, -m}, glm::vec2{du + a, dv}},
        {glm::vec2{+n, +m}, glm::vec2{du + a, dv + b}},
        {glm::vec2{-n, -m}, glm::vec2{du, dv}},
        {glm::vec2{+n, +m}, glm::vec2{du + a, dv + b}},
        {glm::vec2{-n, +m}, glm::vec2{du, dv + b}}
    }};

    this->apply_transform(glm::translate(glm::mat4{1.0f}, glm::vec3{position,0}));
}
