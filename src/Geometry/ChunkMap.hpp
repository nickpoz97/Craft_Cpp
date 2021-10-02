//
// Created by ultimatenick on 02/10/21.
//

#ifndef CPP_CHUNKMAP_HPP
#define CPP_CHUNKMAP_HPP

#include "map_utils.hpp"
#include "Chunk.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

class ChunkMap : public std::unordered_map<glm::ivec2, Chunk>{
public:
    std::array<const Chunk *, 6> chunk_neighbors_pointers(const glm::ivec2 &pq) const;
};


#endif //CPP_CHUNKMAP_HPP
