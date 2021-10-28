//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_TILEBLOCK_HPP
#define CPP_TILEBLOCK_HPP

#include <array>
#include <vector>

#include "glm/vec3.hpp"

enum BlockType{
    EMPTY,
    GRASS,
    SAND,
    STONE,
    BRICK,
    WOOD,
    CEMENT,
    DIRT,
    PLANK,
    SNOW,
    GLASS,
    COBBLE,
    LIGHT_STONE,
    DARK_STONE,
    CHEST,
    LEAVES,
    CLOUD,
    TALL_GRASS,
    YELLOW_FLOWER,
    RED_FLOWER,
    PURPLE_FLOWER,
    SUN_FLOWER,
    WHITE_FLOWER,
    BLUE_FLOWER,
    COLOR_00,
    COLOR_01,
    COLOR_02,
    COLOR_03,
    COLOR_04,
    COLOR_05,
    COLOR_06,
    COLOR_07,
    COLOR_08,
    COLOR_09,
    COLOR_10,
    COLOR_11,
    COLOR_12,
    COLOR_13,
    COLOR_14,
    COLOR_15,
    COLOR_16,
    COLOR_17,
    COLOR_18,
    COLOR_19,
    COLOR_20,
    COLOR_21,
    COLOR_22,
    COLOR_23,
    COLOR_24,
    COLOR_25,
    COLOR_26,
    COLOR_27,
    COLOR_28,
    COLOR_29,
    COLOR_30,
    COLOR_31
};

struct TileCube {
    int leftFace;
    int rightFace;
    int topFace;
    int bottomFace;
    int frontFace;
    int backFace;

    class Iterator : public std::iterator<std::output_iterator_tag, int>{
    public:
        Iterator(const int* tile_pointer);
        int operator*();
        const int* operator->();
        Iterator& operator++();
        bool operator==(const Iterator& b);
        bool operator!=(const Iterator& b);
    private:
        const int* tile_pointer;
    };

    Iterator begin() const;
    Iterator end() const;
};

class TileBlock{
private:
    BlockType tilecube_index{};
public:
    static const std::array<TileCube, 256> tiles;
    [[nodiscard]] BlockType getBlockType() const;

    TileBlock(BlockType block_type);
    TileBlock() = default;
    TileBlock& operator=(const TileBlock& tileBlock) = default;
    TileBlock(const TileBlock& other) = default;

    [[nodiscard]] bool is_plant() const;
    [[nodiscard]] bool is_obstacle() const;
    [[nodiscard]] bool is_transparent() const;
    [[nodiscard]] bool is_destructable() const;
    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] bool is_user_buildable() const;

    operator BlockType() const;

    TileCube::Iterator begin() const;
    TileCube::Iterator end() const;

    static constexpr std::array items{
            GRASS,
            SAND,
            STONE,
            BRICK,
            WOOD,
            CEMENT,
            DIRT,
            PLANK,
            SNOW,
            GLASS,
            COBBLE,
            LIGHT_STONE,
            DARK_STONE,
            CHEST,
            LEAVES,
            TALL_GRASS,
            YELLOW_FLOWER,
            RED_FLOWER,
            PURPLE_FLOWER,
            SUN_FLOWER,
            WHITE_FLOWER,
            BLUE_FLOWER,
            COLOR_00,
            COLOR_01,
            COLOR_02,
            COLOR_03,
            COLOR_04,
            COLOR_05,
            COLOR_06,
            COLOR_07,
            COLOR_08,
            COLOR_09,
            COLOR_10,
            COLOR_11,
            COLOR_12,
            COLOR_13,
            COLOR_14,
            COLOR_15,
            COLOR_16,
            COLOR_17,
            COLOR_18,
            COLOR_19,
            COLOR_20,
            COLOR_21,
            COLOR_22,
            COLOR_23,
            COLOR_24,
            COLOR_25,
            COLOR_26,
            COLOR_27,
            COLOR_28,
            COLOR_29,
            COLOR_30,
            COLOR_31
    };
};

struct Block{
    glm::ivec3 position;
    TileBlock w;
};

#endif //CPP_TILEBLOCK_HPP
