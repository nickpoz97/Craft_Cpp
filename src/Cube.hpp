//
// Created by ultimatenick on 29/07/21.
//

#ifndef CPP_CUBE_HPP
#define CPP_CUBE_HPP

#include <array>
#include <vec3.hpp>
#include <vec2.hpp>

#include "item.hpp"
#include "CubicObject.hpp"
#include "Block.hpp"

class Cube : public CubicObject{
private:
    static glm::vec3 local_vertex_positions[6][4];
    static glm::bvec2 uvs[6][4];
    static int indices[6][6];
    static int flipped[6][6];

    static constexpr float s = 0.0625;              // TODO check semantic
    static constexpr float a = 0.0 + 1 / 2048.0;    // TODO check semantic
    static constexpr float b = s - 1 / 2048.0;      // TODO check semantic

    static constexpr int N_FACES = 6;
    static constexpr int N_VERTICES_FACE = 6;

public:
    Cube(const glm::vec3 &center_position, TileBlock tiles, float ao[6][4], float light[6][4]);
};


#endif //CPP_CUBE_HPP
