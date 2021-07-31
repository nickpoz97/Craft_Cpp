//
// Created by ultimatenick on 29/07/21.
//

#include "cube.hpp"
#include "item.hpp"

// vector size is always 6
Cube::Cube(const Vec<float, 3>& center_position, const Block& tiles) {
    for (int i = 0 ; i < world_vertex_positions.size() ; i++){
        world_vertex_positions[i] = center_position + n * local_vertex_positions[i];
    }
    // TODO handle faces
}
