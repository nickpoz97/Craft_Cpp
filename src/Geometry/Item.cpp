//
// Created by ultimatenick on 03/09/21.
//

#include <ext/scalar_constants.hpp>
#include "map_utils.hpp"
#include "Item.hpp"
#include "CubicObject.hpp"

Item::Item(BlockType w, const glm::vec2 &center_position) :
SuperClass{gen_local_buffer(w, center_position)} {}

std::vector<CubeVertex> Item::gen_local_buffer(BlockType w, const glm::vec2 &center_position) {
    if(TileBlock{w}.is_plant()){
        std::vector<CubeVertex> local_buffer{Plant::max_indices};
        Plant{w, {center_position, 0.0}, -glm::pi<float>() / 4, local_buffer.begin(), {}};
        return local_buffer;
    }
    else {
        std::vector<CubeVertex> local_buffer{Cube::max_indices};
        Cube{w, {1, 1, 1, 1, 1, 1},
             {center_position, 0.0f}, local_buffer.begin(),{}};
        return local_buffer;
    }
}
