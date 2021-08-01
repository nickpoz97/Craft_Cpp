//
// Created by ultimatenick on 29/07/21.
//

#ifndef CPP_CUBE_HPP
#define CPP_CUBE_HPP

#include <array>

#include "item.hpp"
#include "data_structures.hpp"

class Cube{
    static constexpr Vec<float, 3> local_vertex_positions[8]{
            {-1, -1, -1},
            {-1, -1, +1},
            {-1, +1, -1},
            {-1, +1, +1},
            {+1, -1, -1},
            {+1, -1, +1},
            {+1, +1, -1},
            {+1, +1, +1}
    };

    static constexpr Vec<float,3> normals[6]{
            {-1, 0, 0},
            {+1, 0, 0},
            {0, +1, 0},
            {0, -1, 0},
            {0, 0, -1},
            {0, 0, +1}
    };

    static constexpr Vec<bool, 2> uvs[8]{
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1},
            {1, 0},
            {0, 0},
            {1, 1},
            {0, 1}
    };

    static constexpr int indices[6][6]{
            {0,3,2,0,1,3},
            {4,7,5,4,6,7},
            {2,7,6,2,3,7},
            {0,5,1,0,4,5},
            {0,6,4,0,2,6},
            {1,7,3,1,5,7}
    };

    static constexpr float n = 0.5;                 // distance from center
    static constexpr float s = 0.0625;              // TODO check semantic
    static constexpr float a = 0.0 + 1 / 2048.0;    // TODO check semantic
    static constexpr float b = s - 1 / 2048.0;      // TODO check semantic
    static constexpr int n_cube_vertices = 8;
    static constexpr int n_indexed_vertices = 36;

    std::array<Vec<float,3>, n_cube_vertices> world_vertex_positions;
    std::array<Vec<float,2>, n_cube_vertices> vertex_uvs;
    std::array<IndexedVertex, n_indexed_vertices> indexed_vertices;

    Cube(const Vec<float, 3>& center_position, const TileBlock& tiles);

    void setVertexProperties(const Vec<float, 3> &center_position, const TileBlock &tiles);

    void setGeometryProperties();
};


#endif //CPP_CUBE_HPP
