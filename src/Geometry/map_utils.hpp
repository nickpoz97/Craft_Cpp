//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_MAP_UTILS_HPP
#define CPP_MAP_UTILS_HPP

#include <vec3.hpp>
#include <unordered_map>
#include "TileBlock.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

std::size_t hash_int(int key);

template<>
struct std::hash<glm::ivec3>{
    std::size_t operator()(const glm::ivec3& v) const;
};

struct y_coord_comparator{
    bool operator()(const glm::vec3& a, const glm::vec3& b) const;
};

template<>
struct std::hash<glm::ivec2>{
    std::size_t operator()(const glm::ivec2& v) const;
};

#endif //CPP_MAP_UTILS_HPP
