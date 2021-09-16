//
// Created by ultimatenick on 03/09/21.
//

#ifndef CPP_ITEM_HPP
#define CPP_ITEM_HPP


#include "Vertex.hpp"
#include "RenderableEntity.hpp"
#include "GameView.hpp"

class Item : public RenderableEntity<CubeVertex>{
public:
    using SuperClass = RenderableEntity<CubeVertex>;
    explicit Item(BlockType w, const GameView& game_view);
private:
    static std::vector<CubeVertex> gen_local_buffer(BlockType w, int width, int height, int scale);
};


#endif //CPP_ITEM_HPP
