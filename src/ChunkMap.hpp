//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_CHUNKMAP_HPP
#define CPP_CHUNKMAP_HPP

#include "data_structures.hpp"

template<>
struct std::hash<Vec<int,3>>{
    static std::size_t hash_int(int key);

    std::size_t operator()(Vec<int,3> v) const;
};

using BaseMap = std::unordered_map<Vec<int,3>, int>;

class ChunkMap : public BaseMap{
    Vec<int,3> delta;

public:
    ChunkMap(const Vec<int, 3>& delta);
    int& operator[](const Vec<int,3>& key);
};


#endif //CPP_CHUNKMAP_HPP
