//
// Created by ultimatenick on 16/08/21.
//

#ifndef CPP_HASHES_HPP
#define CPP_HASHES_HPP

#include <functional>
#include "vec3.hpp"
#include "vec2.hpp"

std::size_t hash_int(int key);

template<>
struct std::hash<glm::ivec3>{
    std::size_t operator()(const glm::ivec3& v) const;
};

template<>
struct std::hash<glm::ivec2>{
    std::size_t operator()(const glm::ivec2& v) const;
};

#endif //CPP_HASHES_HPP
