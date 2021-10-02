//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "map_utils.hpp"
#include "Chunk.hpp"


std::size_t hash_int(int key) {
    key = ~key + (key << 15);
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = key * 2057;
    key = key ^ (key >> 16);
    return key;
}

std::size_t std::hash<glm::ivec3>::operator()(const glm::ivec3& v) const{
    return hash_int(v.x) ^ hash_int(v.y) ^ hash_int(v.z);
}

std::size_t std::hash<glm::ivec2>::operator()(const glm::ivec2& v) const{
    return hash_int(v.x) ^ hash_int(v.y);
}

bool y_coord_comparator::operator()(const glm::vec3 &a, const glm::vec3 &b) const{
    return a.y < b.y;
}