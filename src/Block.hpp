//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_BLOCK_HPP
#define CPP_BLOCK_HPP

#include <array>

#include "item.hpp"
#include "vec3.hpp"

struct TileBlock {
    int leftFace;
    int rightFace;
    int topFace;
    int bottomFace;
    int frontFace;
    int backFace;

    int get_face_val(unsigned index) const;
    int count_visible_faces() const;
};

class Block{
private:
    static const std::array<TileBlock, 256> tiles;
    const int tile_index;
public:
    explicit Block(Item w);
    [[nodiscard]] const TileBlock& get_tile_block() const;
    int face_val(unsigned index) const;
};


#endif //CPP_BLOCK_HPP
