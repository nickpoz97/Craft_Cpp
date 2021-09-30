//
// Created by ultimatenick on 09/08/21.
//

#include "unordered_map"

#include "BlockMap.hpp"
#include "Chunk.hpp"


std::size_t hash_int(int key) {
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

std::size_t std::hash<glm::ivec2>::operator()(const glm::ivec2& v) const{
    return hash_int(v.x) ^ hash_int(v.y);
}

bool y_coord_comparator::operator()(const glm::vec3 &a, const glm::vec3 &b) const{
    return a.y < b.y;
}

BlockMap::BlockMap(const glm::ivec2 &pq) : delta{pq.x * Chunk::SIZE - 1, 0, pq.y * Chunk::SIZE - 1}{}

glm::ivec2 BlockMap::get_pq() const{
    return {(delta.x + 1) /Chunk::SIZE, (delta.z + 1) /Chunk::SIZE};
}

TileBlock BlockMap::at(const glm::ivec3 &key) const{
    const auto block_it{find(key - delta)};
    return (block_it == cend()) ? TileBlock{} : block_it->second;
}

void BlockMap::set_block(const glm::ivec3 &position, BlockType block_type) {
    //erase(position - delta);
    BaseMap::operator[](position - delta) = TileBlock{block_type};
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

glm::ivec3 BlockMap::get_delta() const {
    return delta;
}

