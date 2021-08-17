//
// Created by ultimatenick on 10/08/21.
//

#include "Chunk.hpp"

void Chunk::draw() {
    buffer.draw_triangles(attrib, faces * 6);
}

int Chunk::get_distance(const Chunk& c1, const Chunk& c2) {
    auto delta = c1.pq - c2.pq;
    return glm::max(glm::abs(delta.x), glm::abs(delta.y));
}

constexpr int Chunk::getSize() {
    return size;
}

int Chunk::getMinY() {
    return min_y;
}

int Chunk::getMaxY() {
    return max_y;
}

