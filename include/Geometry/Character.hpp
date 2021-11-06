//
// Created by ultimatenick on 13/08/21.
//

#ifndef CPP_CHARACTER_HPP
#define CPP_CHARACTER_HPP

#include <array>

#include "Geometry/RenderableEntity.hpp"
#include "Geometry/Vertex.hpp"

namespace CraftCpp {

class Character {
    static constexpr float s = 0.0625;
    static constexpr float a = s;
    static constexpr float b = s * 2;

    static constexpr int nVertices = 6;
    std::array<Uv2DVertex, nVertices> vertices;
public:
    decltype(vertices.begin()) begin();
    decltype(vertices.end()) end();

    Character(const glm::vec2 &position, float m, float n, char c);

    static int getNVertices();
};

}

#endif //CPP_CHARACTER_HPP
