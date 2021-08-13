//
// Created by ultimatenick on 13/08/21.
//

#ifndef CPP_CHARACTER_HPP
#define CPP_CHARACTER_HPP

#include<array>

#include "GameObject.hpp"
#include "StandardVertex.hpp"

class Character : GameObject<UvVertex, 6>{
    static constexpr float s = 0.0625;
    static constexpr float a = s;
    static constexpr float b = s * 2;

    int w, du, dv;

public:
    Character(const glm::vec2 &position, int m, int n, char c);
};


#endif //CPP_CHARACTER_HPP
