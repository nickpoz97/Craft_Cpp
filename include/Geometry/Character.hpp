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

    static constexpr int n_vertices = 6;
    std::array<Uv2DVertex, n_vertices> vertices;
public:
    auto begin() { return vertices.begin(); }

    auto end() { return vertices.end(); }

    Character(const glm::vec2 &position, float m, float n, char c);

    static int get_n_vertices();
};

}

#endif //CPP_CHARACTER_HPP
