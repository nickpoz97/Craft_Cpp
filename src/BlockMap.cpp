//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "BlockMap.hpp"
#include "Chunk.hpp"

BlockMap::BlockMap(const glm::ivec2 &pq) : delta{pq.x * Chunk::size - 1, 0, pq.y * Chunk::size - 1}{}

const glm::ivec3 &BlockMap::get_delta() const{
    return delta;
}

TileBlock BlockMap::get_tileBlock(const glm::ivec3 &key) const{
    const auto block_it{find(key - delta)};
    return (block_it == cend()) ? TileBlock{0} : block_it->second;
}

void BlockMap::set_tileBlock(const glm::ivec3 &key, TileBlock tileBlock) {
    operator[](key - delta) = tileBlock;
}

BlockMap::Iterator::Iterator(const const_iterator& it, const glm::ivec3& delta) : map_it{it}, delta{delta} {}

BlockMap::Iterator::value_type BlockMap::Iterator::operator*() const{
    auto pos_w = *map_it;
    return {pos_w.first + delta, pos_w.second};
}

BlockMap::const_iterator BlockMap::Iterator::operator->() {
    return map_it;
}

BlockMap::Iterator& BlockMap::Iterator::operator++() {
    ++map_it;
    return *this;
}

bool operator==(const BlockMap::Iterator &a, const BlockMap::Iterator &b) {
    return a.map_it == b.map_it;
}

bool operator!=(const BlockMap::Iterator &a, const BlockMap::Iterator &b) {
    return a.map_it != b.map_it;
}

BlockMap::Iterator BlockMap::begin() const{
    return {cbegin(), delta};
}

BlockMap::Iterator BlockMap::end() const{
    return {cend(), delta};
}


