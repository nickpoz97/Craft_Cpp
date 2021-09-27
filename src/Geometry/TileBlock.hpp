//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_TILEBLOCK_HPP
#define CPP_TILEBLOCK_HPP

#include <array>
#include <vector>

#include "vec3.hpp"

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

    [[nodiscard]] int get_face_val(unsigned index) const;
    [[nodiscard]] int count_visible_faces() const;
};

class TileBlock{
private:
    BlockType tile_index{};
public:
    static const std::array<TileCube, 256> tiles;
    [[nodiscard]] BlockType getIndex() const;

    explicit TileBlock(BlockType block_type);
    TileBlock() = default;
    TileBlock& operator=(const TileBlock& tileBlock) = default;
    TileBlock(const TileBlock& other) = default;
    [[nodiscard]] int face_tile(unsigned index) const;

    [[nodiscard]] bool is_plant() const;
    [[nodiscard]] bool is_obstacle() const;
    [[nodiscard]] bool is_transparent() const;
    [[nodiscard]] bool is_destructable() const;
    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] bool is_user_buildable() const;

    explicit operator int() const;

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
