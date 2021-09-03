//
// Created by ultimatenick on 03/09/21.
//

#include "Item.hpp"
#include "CubicObject.hpp"

Item::Item(TileBlock w) : SuperClass{gen_local_buffer(w)} {}

std::vector<CubeVertex> Item::gen_local_buffer(TileBlock w) {
    if(w.is_plant()){
        std::vector<CubeVertex> local_buffer{Plant::max_indices};
        Plant{w, {1,1,1,1,1,1}, {}, 45, local_buffer.begin()};
        return local_buffer;
    }
    else {
        std::vector<CubeVertex> local_buffer{Cube::max_indices};
        Cube{w, {1, 1, 1, 1, 1, 1}, {}, local_buffer.begin()};
        return local_buffer;
    }
}
