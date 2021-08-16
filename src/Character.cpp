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
                        Uv3DVertex{glm::vec4{-n, -m, 0, 0}, glm::vec2{du, dv}},
                        Uv3DVertex{glm::vec4{+n, -m, 0, 0}, glm::vec2{du + a, dv}},
                        Uv3DVertex{glm::vec4{+n, +m, 0, 0}, glm::vec2{du + a, dv + b}},
                        Uv3DVertex{glm::vec4{-n, -m, 0, 0}, glm::vec2{du, dv}},
                        Uv3DVertex{glm::vec4{+n, +m, 0, 0}, glm::vec2{du + a, dv + b}},
                        Uv3DVertex{glm::vec4{-n, +m, 0, 0}, glm::vec2{du, dv + b}}
    }};

    this->apply_transform(glm::translate(glm::mat4{1.0f}, glm::vec3{position,0}));
    this->set_buffer_data();
}
