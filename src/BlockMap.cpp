//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "BlockMap.hpp"

BlockMap::BlockMap(const glm::ivec3& delta) : BaseMap(), delta{delta}{}

int &BlockMap::at(const glm::ivec3 &key){
    return BaseMap::at(key - delta);
}

std::size_t std::hash<glm::ivec3>::hash_int(int key) {
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
