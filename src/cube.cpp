//
// Created by ultimatenick on 29/07/21.
//

#include "cube.hpp"
#include "item.hpp"
#include "data_structures.hpp"

Cube::Cube(const Vec<float, 3> &center_position, int tileIndex, float ao[6][4], float light[6][4]) {
    const Block tile{tileIndex};
    for (int i = 0; i < n_faces; i++) {
        for (int j = 0; j < n_vertices_face; j++) {
            bool flip = ao[i][0] + ao[i][3] > ao[i][1] + ao[i][2];

            auto &v = vertices[i][j];
            v.position = center_position + (n * local_vertex_positions[i][j]);
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