//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "ChunkMap.hpp"

ChunkMap::ChunkMap(const Vec<int, 3>& delta) : BaseMap(), delta{delta}{}

int &ChunkMap::operator[](const Vec<int, 3> &key) {
    auto it = BaseMap::find(key - delta);
    return it->second;
}

std::size_t std::hash<Vec<int,3>>::hash_int(int key) {
    key = ~key + (key << 15);
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = key * 2057;
    key = key ^ (key >> 16);
    return key;
}

std::size_t std::hash<Vec<int,3>>::operator()(Vec<int, 3> v) const{
    return hash_int(v.x) ^ hash_int(v.y) ^ hash_int(v.z);
}
