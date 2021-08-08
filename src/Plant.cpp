//
// Created by ultimatenick on 06/08/21.
//

#include "Plant.hpp"

Plant::Plant(const Vec<float, 3> &center_position, int tileIndex, float ao, float light) :
GameObject(n_faces, n_vertices_face){
    const int tile{tileIndex};
    for (int i = 0; i < n_faces; i++) {
        for (int j = 0; j < n_vertices_face; j++) {
            auto &v = vertices[n_vertices_face * i + j];
            v.setPosition(Vec<float,4> {center_position + (n * local_vertex_positions[i][j]), 1});
            v.setFace(static_cast<Face>(i));
            v.index = static_cast<int>(indices[i][j]);
            v.uv = {
                    static_cast<float>(tile % 16) * s + (uvs[i][j].x ? b : a),
                    static_cast<float>(tile / 16) * s + (uvs[i][j].x ? b : a),
            };
            v.ao = ao;
            v.light = light;
        }
    }
}
