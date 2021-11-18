//
// Created by ultimatenick on 03/09/21.
//

#include "glm/ext/scalar_constants.hpp"
#include "Geometry/mapUtils.hpp"
#include "Geometry/Item.hpp"
#include "Geometry/BlockObject.hpp"

namespace CraftCpp {
Item::Item(BlockType w, const glm::vec2 &centerPosition) :
        SuperClass{genLocalBuffer(w, centerPosition)} {}

std::vector<BlockVertex> Item::genLocalBuffer(BlockType w, const glm::vec2 &center_position) {
    if (TileBlock{w}.isPlant()) {
        std::vector<BlockVertex> local_buffer{Plant::maxIndices};
        Plant{w, {center_position, 0.0}, -glm::pi<float>() / 4, local_buffer.begin(), {}};
        return local_buffer;
    } else {
        std::vector<BlockVertex> local_buffer{Cube::maxIndices};
        Cube{w, {1, 1, 1, 1, 1, 1},
             {center_position, 0.0f}, local_buffer.begin(), {}};
        return local_buffer;
    }
}
}