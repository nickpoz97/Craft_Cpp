//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "BlockMap.hpp"

BlockMap::BlockMap(const glm::ivec3& delta) : BaseMap(), delta{delta}{}

int &BlockMap::at(const glm::ivec3 &key){
    return BaseMap::at(key - delta);
}

