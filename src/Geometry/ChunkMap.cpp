//
// Created by ultimatenick on 02/10/21.
//

#include "ChunkMap.hpp"

std::array<const Chunk *, 6> ChunkMap::chunk_neighbors_pointers(const glm::ivec2 &pq) const {
    std::array<const Chunk *, 6> neighbors{};
    auto it  = neighbors.begin();

    for(int dp = -1 ; dp <= 1 ; dp++){
        for(int dq = -1 ; dq <= 1 ; dq++){
            auto result_it{find({pq.x - dp, pq.y - dq})};
            *(it++) = (result_it != end()) ? &(result_it->second) : nullptr;
        }
    }

    return neighbors;
}
