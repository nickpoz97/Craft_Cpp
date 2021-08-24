//
// Created by ultimatenick on 15/08/21.
//

#ifndef CPP_CHARACTER3D_HPP
#define CPP_CHARACTER3D_HPP


#include <array>
#include "GameObject.hpp"
#include "Vertex.hpp"
#include "item.hpp"

class Character3D : GameObject<Uv3DVertex, 6>{
private:
    char c;
public:
    static float constexpr s = 0.0625;
    static float constexpr p = 0.5;
    static float constexpr pu = s / 5;
    static float constexpr pv = s / 2.5;

    static const std::array<std::array<glm::vec3, 6>, 8> local_positions;
    static const std::array<std::array<glm::vec2, 6>, 8> uvs;
    static const std::array<glm::vec3,8> offsets;

    Character3D(const glm::vec3& position, float n, int face, char c));
};


#endif //CPP_CHARACTER3D_HPP
