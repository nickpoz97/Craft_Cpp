//
// Created by ultimatenick on 10/08/21.
//

#include "Chunk.hpp"

void Chunk::draw() {
    buffer.draw_triangles(attrib, faces * 6);
}
