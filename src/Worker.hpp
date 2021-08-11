//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_WORKER_HPP
#define CPP_WORKER_HPP

#include <vec2.hpp>
#include <array>

#include "BlockMap.hpp"

class WorkerItem {
private:
    glm::ivec2 coordinates;
    int miny;
    int maxy;

    int load;
    std::array<std::array<BlockMap,3>,3> block_maps;
    std::array<std::array<BlockMap,3>,3> light_maps;
};

class Worker{
    int index;
    int state;

    WorkerItem item;
};

#endif //CPP_WORKER_HPP
