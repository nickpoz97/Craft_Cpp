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
public:
    const glm::ivec2 pq_coordinates;
    int miny{};
    int maxy{};

    //const bool load;
    int n_faces{};

    std::array<std::array<std::shared_ptr<BlockMap>,3>,3> block_maps{};
    //std::array<std::array<BlockMap,3>,3> light_maps;

    bool has_light() const;
    WorkerItem(const Chunk& pq);
};

class Worker{
    int index;
    int state;

    WorkerItem item;
};

#endif //CPP_WORKER_HPP
