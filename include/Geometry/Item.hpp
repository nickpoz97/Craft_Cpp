//
// Created by ultimatenick on 03/09/21.
//

#ifndef CPP_ITEM_HPP
#define CPP_ITEM_HPP


#include "Geometry/Vertex.hpp"
#include "Geometry/RenderableEntity.hpp"
#include "Interaction/GameView.hpp"

namespace CraftCpp {

/// @brief UI version o a BlockObject
/// @warning never been tested
class Item : public RenderableEntity<BlockVertex> {
private:
    static std::vector<BlockVertex> genLocalBuffer(BlockType w, const glm::vec2 &center_position);

    using SuperClass = RenderableEntity<BlockVertex>;
public:
    /**
     * @brief builds an Item object on the screen
     * @param[in] w type of item
     * @param[in] centerPosition 2D screen coordinates
     */
    explicit Item(BlockType w, const glm::vec2 &centerPosition);
};
}

#endif //CPP_ITEM_HPP
