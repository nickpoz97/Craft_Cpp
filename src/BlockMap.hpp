//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_BLOCKMAP_HPP
#define CPP_BLOCKMAP_HPP

#include "Vec.hpp"

template<>
struct std::hash<Vec<int,3>>{
    static std::size_t hash_int(int key);

    std::size_t operator()(Vec<int,3> v) const;
};

using BaseMap = std::unordered_map<Vec<int,3>, int>;

class BlockMap : public BaseMap{
    Vec<int,3> delta;

public:
    BlockMap(const Vec<int, 3>& delta);
    int& at(const Vec<int,3>& key);
};


#endif //CPP_BLOCKMAP_HPP
