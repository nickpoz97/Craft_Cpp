//
// Created by ultimatenick on 13/08/21.
//

#ifndef CPP_CHARACTER_HPP
#define CPP_CHARACTER_HPP

#include<array>

#include "GameObject.hpp"
#include "Vertex.hpp"

class Character : public GameObject<Uv2DVertex, 6>{
    static constexpr float s = 0.0625;
    static constexpr float a = s;
    static constexpr float b = s * 2;

    int w, du, dv;

public:
    Character(const glm::vec2 &position, int m, int n, char c);
};


#endif //CPP_CHARACTER_HPP
