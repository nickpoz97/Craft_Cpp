//
// Created by ultimatenick on 29/07/21.
//

#include "cube.hpp"
#include "item.hpp"

/*Cube::Cube(int nComponents, int nFaces, Vec<float, 3> position) :
    data{new float[6 * nComponents * nFaces]},
    position{position}
    {}

Cube::~Cube(){
    delete[] data;
}

void Cube::makeCubeFaces(Block tiles, Block faces, CubeLighting ao, CubeLighting light, float n) {
    float* d = data;

    for (int i = 0; i < 6; i++) {
        if (faces[i] == 0) {
            continue;
        }
        float du = (tiles[i] % 16) * s;
        float dv = (tiles[i] / 16) * s;

        int flip = ao[i][0] + ao[i][3] > ao[i][1] + ao[i][2];
        for (int v = 0; v < 6; v++) {
            int j = flip ? flipped[i][v] : indices[i][v];
            *(d++) = position.x + n * positions[i][j].x;
            *(d++) = position.y + n * positions[i][j].y;
            *(d++) = position.z + n * positions[i][j].z;
            *(d++) = normals[i].x;
            *(d++) = normals[i].y;
            *(d++) = normals[i].z;
            *(d++) = du + (uvs[i][j].x ? b : a);
            *(d++) = dv + (uvs[i][j].y ? b : a);
            *(d++) = ao[i][j];
            *(d++) = light[i][j];
        }
    }
}
*/