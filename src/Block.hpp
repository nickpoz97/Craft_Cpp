//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_BLOCK_HPP
#define CPP_BLOCK_HPP

#include "data_structures.hpp"

class Block{
private:
    static TileBlock cubeTiles[];
    static int plantTiles[];

    const int index;
public:
    explicit Block(int index);
    [[nodiscard]] const TileBlock& getBlock() const;
    int operator[] (int faceIndex) const;
};


#endif //CPP_BLOCK_HPP
