//
// Created by ultimatenick on 29/07/21.
//

#ifndef CPP_CUBE_HPP
#define CPP_CUBE_HPP

#include <array>

#include "item.hpp"
#include "data_structures.hpp"

class Cube{
    static constexpr Vec<float, 3> local_vertex_positions[6][4]{
            {{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1}},
            {{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}},
            {{-1, +1, -1}, {-1, +1, +1}, {+1, +1, -1}, {+1, +1, +1}},
            {{-1, -1, -1}, {-1, -1, +1}, {+1, -1, -1}, {+1, -1, +1}},
            {{-1, -1, -1}, {-1, +1, -1}, {+1, -1, -1}, {+1, +1, -1}},
            {{-1, -1, +1}, {-1, +1, +1}, {+1, -1, +1}, {+1, +1, +1}}
    };

    static constexpr Vec<float,3> normals[6]{
            {-1, 0, 0},
            {+1, 0, 0},
            {0, +1, 0},
            {0, -1, 0},
            {0, 0, -1},
            {0, 0, +1}
    };

    static constexpr Vec<bool, 2> uvs[6][4]{
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{1, 0}, {0, 0}, {1, 1}, {0, 1}},
            {{0, 1}, {0, 0}, {1, 1}, {1, 0}},
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
            {{1, 0}, {1, 1}, {0, 0}, {0, 1}}
    };

    static constexpr int indices[6][6]{
            {0, 3, 2, 0, 1, 3},
            {0, 3, 1, 0, 2, 3},
            {0, 3, 2, 0, 1, 3},
            {0, 3, 1, 0, 2, 3},
            {0, 3, 2, 0, 1, 3},
            {0, 3, 1, 0, 2, 3}
    };

    static constexpr int flipped[6][6] = {
            {0, 1, 2, 1, 3, 2},
            {0, 2, 1, 2, 3, 1},
            {0, 1, 2, 1, 3, 2},
            {0, 2, 1, 2, 3, 1},
            {0, 1, 2, 1, 3, 2},
            {0, 2, 1, 2, 3, 1}
    };

    static constexpr float n = 0.5;                 // distance from center
    static constexpr float s = 0.0625;              // TODO check semantic
    static constexpr float a = 0.0 + 1 / 2048.0;    // TODO check semantic
    static constexpr float b = s - 1 / 2048.0;      // TODO check semantic
    static constexpr int n_faces = 6;
    static constexpr int n_vertices_face = 6;

    std::array<std::array<Vertex, n_vertices_face>, n_faces> vertices;

    Cube(const Vec<float, 3> &center_position, const TileBlock &tiles, float ao[6][4], float light[6][4]);
};


#endif //CPP_CUBE_HPP
