//
// Created by ultimatenick on 03/09/21.
//

#include <ext/scalar_constants.hpp>
#include "Item.hpp"
#include "CubicObject.hpp"

Item::Item(BlockType w) :
SuperClass{gen_local_buffer(w)} {}

std::vector<CubeVertex> Item::gen_local_buffer(BlockType w) {
    if(TileBlock{w}.is_plant()){
        std::vector<CubeVertex> local_buffer{Plant::max_indices};
        Plant{w, {1,1,1,1,1,1},
              {}, -glm::pi<float>() / 4, local_buffer.begin()};
        return local_buffer;
    }
    else {
        std::vector<CubeVertex> local_buffer{Cube::max_indices};
        Cube{w, {1, 1, 1, 1, 1, 1},
             {}, local_buffer.begin()};
        return local_buffer;
    }
}
