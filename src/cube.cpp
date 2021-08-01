//
// Created by ultimatenick on 29/07/21.
//

#include "cube.hpp"
#include "item.hpp"
#include "data_structures.hpp"

Cube::Cube(const Vec<float, 3>& center_position, const TileBlock& tiles) {
    setVertexProperties(center_position, tiles);
    setGeometryProperties();
}

void Cube::setGeometryProperties() {
    for (int i = 0 ; i < 6 ; i++){
        for (int j = 0 ; j < 6 ; j++){
            auto iv = indexed_vertices[i + j];
            iv.face = static_cast<Face>(i);
            iv.index = j;
            iv.world_position_index = indices[i][j];
        }
    }
}

void Cube::setVertexProperties(const Vec<float, 3> &center_position, const TileBlock &tiles) {
    for (int i = 0 ; i < n_cube_vertices ; i++){
        world_vertex_positions[i] = center_position + (n * local_vertex_positions[i]);
        vertex_uvs = {
                static_cast<float>(tiles[i] % 16) * s + (uvs[i].x ? b : a),
                static_cast<float>(tiles[i] / 16) * s + (uvs[i].x ? b : a),
        };
    }
}
