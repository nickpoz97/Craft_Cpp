//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_BLOCKMAP_HPP
#define CPP_BLOCKMAP_HPP

#include <vec3.hpp>
#include <map>
#include "map_utils.hpp"
#include "item.hpp"

// value is w
using BaseMap = std::map<glm::ivec3, Item, y_coord_comparator>;

class BlockMap : public BaseMap{
    glm::ivec3 delta;

public:
    explicit BlockMap(const glm::ivec3& delta);
    Item at(const glm::ivec3& key) const;
};


#endif //CPP_BLOCKMAP_HPP
