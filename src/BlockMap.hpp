//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_BLOCKMAP_HPP
#define CPP_BLOCKMAP_HPP

#include <vec3.hpp>

template<>
struct std::hash<glm::ivec3>{
    static std::size_t hash_int(int key);

    std::size_t operator()(const glm::ivec3& v) const;
};

// value is w
using BaseMap = std::unordered_map<glm::ivec3, int>;

class BlockMap : public BaseMap{
    glm::ivec3 delta;

public:
    BlockMap(const glm::ivec3& delta);
    int& at(const glm::ivec3& key);
};


#endif //CPP_BLOCKMAP_HPP
