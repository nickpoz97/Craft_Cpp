//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_WORKER_HPP
#define CPP_WORKER_HPP

#include <vec2.hpp>
#include <array>

#include "BlockMap.hpp"
#include "Chunk.hpp"

struct WorkerItem {
    const glm::ivec2 pq_coordinates;
    int miny{};
    int maxy{};
    static bool is_on_edge(int x, int z);

    //const bool load;
    int n_faces{};

    const std::array<std::array<BlockMap,3>,3> block_maps{};
    //const std::array<std::array<BlockMap,3>,3> light_maps{};

    bool has_light() const;
    WorkerItem(const glm::ivec2& pq);
};

class Worker{
    int index;
    int state;

    WorkerItem item;
};

#endif //CPP_WORKER_HPP
