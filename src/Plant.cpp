//
// Created by ultimatenick on 06/08/21.
//

#include "Plant.hpp"

Plant::Plant(const Vec<float, 3> &center_position, int tileIndex, float ao, float light) {
    const int tile{tileIndex};
    for (int i = 0; i < n_faces; i++) {
        for (int j = 0; j < n_vertices_face; j++) {
            auto &v = vertices[i][j];
            v.position = center_position + (n * local_vertex_positions[i][j]);
            v.face = static_cast<Face>(i);
            v.index = indices[i][j];
            v.uv = {
                    static_cast<float>(tile % 16) * s + (uvs[i][j].x ? b : a),
                    static_cast<float>(tile / 16) * s + (uvs[i][j].x ? b : a),
            };
            v.ao = ao;
            v.light = light;
        }
    }
}
