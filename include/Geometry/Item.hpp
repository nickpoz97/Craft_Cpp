//
// Created by ultimatenick on 03/09/21.
//

#ifndef CPP_ITEM_HPP
#define CPP_ITEM_HPP


#include "Geometry/Vertex.hpp"
#include "Geometry/RenderableEntity.hpp"
#include "Interaction/GameView.hpp"

namespace CraftCpp {
class Item : public RenderableEntity<BlockVertex> {
public:
    using SuperClass = RenderableEntity<BlockVertex>;

    explicit Item(BlockType w, const glm::vec2 &center_position);

private:
    static std::vector<BlockVertex> gen_local_buffer(BlockType w, const glm::vec2 &center_position);
};
}

#endif //CPP_ITEM_HPP
