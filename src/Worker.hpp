//
// Created by ultimatenick on 10/08/21.
//

#ifndef CPP_WORKER_HPP
#define CPP_WORKER_HPP

#include "Vec.hpp"
#include "BlockMap.hpp"

class WorkerItem {
private:
    Vec<int,2> coordinates;
    Vec<int,2> min;

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
