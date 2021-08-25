//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "BlockMap.hpp"

BlockMap::BlockMap(const glm::ivec3& delta) : delta{delta}{}

const glm::ivec3 &BlockMap::get_delta() const{
    return delta;
}

Item BlockMap::get_item(const glm::ivec3 &key) const{
    auto block_it{map.find(key - delta)};
    return (block_it == map.end()) ? Item{} : block_it->second;
}

void BlockMap::set_item(const glm::ivec3 &key, Item item) {
    map[key - delta] = item;
}

BlockMap::Iterator::Iterator(const BaseMap::const_iterator& it, const glm::ivec3& delta) : map_it{it}, delta{delta} {}

BlockMap::Iterator::value_type BlockMap::Iterator::operator*(){
    auto pos_w = *map_it;
    return {pos_w.first + delta, pos_w.second};
}

std::map<glm::ivec3, Item, y_coord_comparator>::const_iterator BlockMap::Iterator::operator->() {
    return map_it;
}

BlockMap::Iterator BlockMap::Iterator::operator++() {
    ++map_it;
    return *this;
}

bool operator==(const BlockMap::Iterator &a, const BlockMap::Iterator &b) {
    return a.map_it == b.map_it;
}

bool operator!=(const BlockMap::Iterator &a, const BlockMap::Iterator &b) {
    return a.map_it != b.map_it;
}

BlockMap::Iterator BlockMap::begin() {
    return {map.cbegin(), delta};
}

BlockMap::Iterator BlockMap::end() {
    return {map.cend(), delta};
}

