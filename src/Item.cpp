//
// Created by ultimatenick on 03/09/21.
//

#include <ext/scalar_constants.hpp>
#include "Item.hpp"
#include "CubicObject.hpp"

Item::Item(BlockType w, const GameView& game_view) :
SuperClass{
    gen_local_buffer(w, game_view.get_width(), game_view.get_height(),  game_view.get_scale())} {}

std::vector<CubeVertex> Item::gen_local_buffer(BlockType w, int width, int height, int scale) {
    float ratio = static_cast<float>(width) / height;
    float size = 64 * scale;
    float box = height / size * 2;
    auto get_offset = [size, width](int axis){return 1 - size / width * 2;};

    glm::vec2 offset{get_offset(width), get_offset(height)};

    if(TileBlock{w}.is_plant()){
        std::vector<CubeVertex> local_buffer{Plant::max_indices};
        Plant{w, {1,1,1,1,1,1},
              {offset, 0.0}, -glm::pi<float>() / 4, local_buffer.begin()};
        return local_buffer;
    }
    else {
        std::vector<CubeVertex> local_buffer{Cube::max_indices};
        Cube{w, {1, 1, 1, 1, 1, 1},
             {offset, 0.0}, local_buffer.begin()};
        return local_buffer;
    }
}
