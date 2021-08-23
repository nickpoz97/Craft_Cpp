//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "BlockMap.hpp"

BlockMap::BlockMap(const glm::ivec3& delta) : BaseMap(), delta{delta}{}

const glm::ivec3 &BlockMap::get_delta() const{
    return delta;
}

Item BlockMap::at(const glm::ivec3 &key) const{
    auto block_it{this->find(key)};
    return (block_it == this->end()) ? Item{} : block_it->second;
}

