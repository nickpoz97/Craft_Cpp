//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "Geometry/map_utils.hpp"
#include "Geometry/Chunk.hpp"

namespace CraftCpp {
std::size_t hash_coefficient(int val) {
    std::size_t key = static_cast<std::size_t>(val);
    key = ~key + (key << 15);
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = key * 2057;
    key = key ^ (key >> 16);
    return key;
}

bool y_coord_comparator::operator()(const glm::vec3 &a, const glm::vec3 &b) const {
    return a.y < b.y;
}
}

std::size_t std::hash<glm::ivec3>::operator()(const glm::ivec3 &v) const {
    return CraftCpp::hash_coefficient(v.x) ^ CraftCpp::hash_coefficient(v.y) ^ CraftCpp::hash_coefficient(v.z);
}

std::size_t std::hash<glm::ivec2>::operator()(const glm::ivec2 &v) const {
    return CraftCpp::hash_coefficient(v.x) ^ CraftCpp::hash_coefficient(v.y);
}
