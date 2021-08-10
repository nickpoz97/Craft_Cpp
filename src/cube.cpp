//
// Created by ultimatenick on 29/07/21.
//

#include "cube.hpp"
#include "Block.hpp"
#include "Vec.hpp"

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

Cube::Cube(const Vec<float, 3> &center_position, TileBlock tiles, float ao[6][4], float light[6][4]) :
GameObject(N_FACES, N_VERTICES_FACE){

    for (int i = 0; i < N_FACES; i++) {
        for (int j = 0; j < N_VERTICES_FACE; j++) {
            bool flip = ao[i][0] + ao[i][3] > ao[i][1] + ao[i][2];

            auto &v = vertices[i][j];
            v.setPosition(Vec<float,4> {center_position + (n * local_vertex_positions[i][j]), 1});

            v.setFace(static_cast<Face>(i));
            v.setIndex(flip ? flipped[i][j] : indices[i][j]);
            v.setUv({static_cast<float>(tiles[i] % 16) * s + (uvs[i][j].x ? b : a),
                    static_cast<float>(tiles[i] / 16) * s + (uvs[i][j].x ? b : a)}
            );
            v.setAo(ao[i][v.getIndex()]);
            v.setLight(light[i][v.getIndex()]);
        }
    }
}