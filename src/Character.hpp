//
// Created by ultimatenick on 13/08/21.
//

#ifndef CPP_CHARACTER_HPP
#define CPP_CHARACTER_HPP

#include<array>

#include "GameObject.hpp"
#include "Vertex.hpp"

class Character{
    static constexpr float s = 0.0625;
    static constexpr float a = s;
    static constexpr float b = s * 2;

    std::array<Uv2DVertex, 6> vertices;
public:
    auto begin() {return vertices.begin();}
    auto end() {return vertices.end();}

public:
    Character(const glm::vec2 &position, float m, float n, char c);
};


#endif //CPP_CHARACTER_HPP
