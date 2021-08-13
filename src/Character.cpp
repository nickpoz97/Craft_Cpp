//
// Created by ultimatenick on 13/08/21.
//

#include "Character.hpp"
#include "ext/matrix_transform.hpp"

Character::Character(const glm::vec2 &position, int m, int n, char c) : GameObject<UvVertex, 6>(){
    int w = c - 32;

    float du = (w % 16) * a;
    float dv = 1 - (w / 16) * b - b;

    vertices = {{
        UvVertex{glm::vec4{-n,-m,0,0},glm::vec2{du,dv}},
        UvVertex{glm::vec4{+n,-m,0,0},glm::vec2{du+a,dv}},
        UvVertex{glm::vec4{+n,+m,0,0},glm::vec2{du+a,dv+b}},
        UvVertex{glm::vec4{-n,-m,0,0},glm::vec2{du,dv}},
        UvVertex{glm::vec4{+n,+m,0,0},glm::vec2{du+a,dv+b}},
        UvVertex{glm::vec4{-n,+m,0,0},glm::vec2{du,dv+b}}
    }};

    this->apply_transform(glm::translate(glm::mat4{1.0f}, glm::vec3{position,0}));
    this->set_buffer_data();
}
