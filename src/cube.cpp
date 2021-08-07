//
// Created by ultimatenick on 29/07/21.
//

#include "cube.hpp"
#include "Block.hpp"
#include "data_structures.hpp"

Vec<float, 3> Cube::local_vertex_positions[6][4]{
    {{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1}},
    {{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}},
    {{-1, +1, -1}, {-1, +1, +1}, {+1, +1, -1}, {+1, +1, +1}},
    {{-1, -1, -1}, {-1, -1, +1}, {+1, -1, -1}, {+1, -1, +1}},
    {{-1, -1, -1}, {-1, +1, -1}, {+1, -1, -1}, {+1, +1, -1}},
    {{-1, -1, +1}, {-1, +1, +1}, {+1, -1, +1}, {+1, +1, +1}}
};

Vec<bool, 2> Cube::uvs[6][4]{
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
    {{1, 0}, {0, 0}, {1, 1}, {0, 1}},
    {{0, 1}, {0, 0}, {1, 1}, {1, 0}},
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    {{1, 0}, {1, 1}, {0, 0}, {0, 1}}
};

int Cube::indices[6][6]{
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3},
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3},
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3}
};

int Cube::flipped[6][6] = {
        {0, 1, 2, 1, 3, 2},
        {0, 2, 1, 2, 3, 1},
        {0, 1, 2, 1, 3, 2},
        {0, 2, 1, 2, 3, 1},
        {0, 1, 2, 1, 3, 2},
        {0, 2, 1, 2, 3, 1}
};

Cube::Cube(const Vec<float, 3> &center_position, int tileIndex, float ao[6][4], float light[6][4]) :
GameObject(n_faces, n_vertices_face){
    const Block tile{tileIndex};

    for (int i = 0; i < n_faces; i++) {
        for (int j = 0; j < n_vertices_face; j++) {
            bool flip = ao[i][0] + ao[i][3] > ao[i][1] + ao[i][2];

            auto &v = vertices[i][j];
            v.position = Vec<float,4>{center_position + (n * local_vertex_positions[i][j]), 1 };
            v.face = static_cast<Face>(i);
            v.index = flip ? flipped[i][j] : indices[i][j];
            v.uv = {
                    static_cast<float>(tile[i] % 16) * s + (uvs[i][j].x ? b : a),
                    static_cast<float>(tile[i] / 16) * s + (uvs[i][j].x ? b : a),
            };
            v.ao = ao[i][v.index];
            v.light = light[i][v.index];
        }
    }
}