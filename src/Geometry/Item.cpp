//
// Created by ultimatenick on 03/09/21.
//

#include "glm/ext/scalar_constants.hpp"
#include "Geometry/map_utils.hpp"
#include "Geometry/Item.hpp"
#include "Geometry/BlockObject.hpp"

namespace CraftCpp {
Item::Item(BlockType w, const glm::vec2 &center_position) :
        SuperClass{gen_local_buffer(w, center_position)} {}

std::vector<CubeVertex> Item::gen_local_buffer(BlockType w, const glm::vec2 &center_position) {
    if (TileBlock{w}.isPlant()) {
        std::vector<CubeVertex> local_buffer{Plant::maxIndices};
        Plant{w, {center_position, 0.0}, -glm::pi<float>() / 4, local_buffer.begin(), {}};
        return local_buffer;
    } else {
        std::vector<CubeVertex> local_buffer{Cube::maxIndices};
        Cube{w, {1, 1, 1, 1, 1, 1},
             {center_position, 0.0f}, local_buffer.begin(), {}};
        return local_buffer;
    }
}
}