//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_BLOCK_HPP
#define CPP_BLOCK_HPP

#include <vec3.hpp>

struct TileBlock{
    int leftFace;
    int rightFace;
    int topFace;
    int bottomFace;
    int frontFace;
    int backFace;

    int operator[](int i) const;
};

class Block{
private:
    static TileBlock cubeTiles[];
    static int plantTiles[];

    const int tile_index;
    glm::vec3 position;
public:
    explicit Block(int tile_index, const glm::vec3& position);
    [[nodiscard]] const TileBlock& getBlock() const;
    int operator[] (int faceIndex) const;
};


#endif //CPP_BLOCK_HPP
