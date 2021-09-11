//
// Created by ultimatenick on 07/08/21.
//

#include "TileBlock.hpp"

decltype(TileBlock::tiles) TileBlock::tiles{{
    {0, 0, 0, 0, 0, 0}, // 0 - empty
    {16, 16, 32, 0, 16, 16}, // 1 - grass
    {1, 1, 1, 1, 1, 1}, // 2 - sand
    {2, 2, 2, 2, 2, 2}, // 3 - stone
    {3, 3, 3, 3, 3, 3}, // 4 - brick
    {20, 20, 36, 4, 20, 20}, // 5 - wood
    {5, 5, 5, 5, 5, 5}, // 6 - cement
    {6, 6, 6, 6, 6, 6}, // 7 - dirt
    {7, 7, 7, 7, 7, 7}, // 8 - plank
    {24, 24, 40, 8, 24, 24}, // 9 - snow
    {9, 9, 9, 9, 9, 9}, // 10 - glass
    {10, 10, 10, 10, 10, 10}, // 11 - cobble
    {11, 11, 11, 11, 11, 11}, // 12 - light stone
    {12, 12, 12, 12, 12, 12}, // 13 - dark stone
    {13, 13, 13, 13, 13, 13}, // 14 - chest
    {14, 14, 14, 14, 14, 14}, // 15 - leaves
    {15, 15, 15, 15, 15, 15}, // 16 - cloud
    {48, 48, 0, 0, 48, 48}, // 17 - tall grass
    {49, 49, 0, 0, 49, 49}, // 18 - yellow flower
    {50, 50, 0, 0, 50, 50}, // 19 - red flower
    {51, 51, 0, 0, 51, 51}, // 20 - purple flower
    {52, 52, 0, 0, 52, 52}, // 21 - sun flower
    {53, 53, 0, 0, 53, 53}, // 22 - white flower
    {54, 54, 0, 0, 54, 54}, // 23 - blue flower
    {0, 0, 0, 0, 0, 0}, // 24
    {0, 0, 0, 0, 0, 0}, // 25
    {0, 0, 0, 0, 0, 0}, // 26
    {0, 0, 0, 0, 0, 0}, // 27
    {0, 0, 0, 0, 0, 0}, // 28
    {0, 0, 0, 0, 0, 0}, // 29
    {0, 0, 0, 0, 0, 0}, // 30
    {0, 0, 0, 0, 0, 0}, // 31
    {176, 176, 176, 176, 176, 176}, // 32
    {177, 177, 177, 177, 177, 177}, // 33
    {178, 178, 178, 178, 178, 178}, // 34
    {179, 179, 179, 179, 179, 179}, // 35
    {180, 180, 180, 180, 180, 180}, // 36
    {181, 181, 181, 181, 181, 181}, // 37
    {182, 182, 182, 182, 182, 182}, // 38
    {183, 183, 183, 183, 183, 183}, // 39
    {184, 184, 184, 184, 184, 184}, // 40
    {185, 185, 185, 185, 185, 185}, // 41
    {186, 186, 186, 186, 186, 186}, // 42
    {187, 187, 187, 187, 187, 187}, // 43
    {188, 188, 188, 188, 188, 188}, // 44
    {189, 189, 189, 189, 189, 189}, // 45
    {190, 190, 190, 190, 190, 190}, // 46
    {191, 191, 191, 191, 191, 191}, // 47
    {192, 192, 192, 192, 192, 192}, // 48
    {193, 193, 193, 193, 193, 193}, // 49
    {194, 194, 194, 194, 194, 194}, // 50
    {195, 195, 195, 195, 195, 195}, // 51
    {196, 196, 196, 196, 196, 196}, // 52
    {197, 197, 197, 197, 197, 197}, // 53
    {198, 198, 198, 198, 198, 198}, // 54
    {199, 199, 199, 199, 199, 199}, // 55
    {200, 200, 200, 200, 200, 200}, // 56
    {201, 201, 201, 201, 201, 201}, // 57
    {202, 202, 202, 202, 202, 202}, // 58
    {203, 203, 203, 203, 203, 203}, // 59
    {204, 204, 204, 204, 204, 204}, // 60
    {205, 205, 205, 205, 205, 205}, // 61
    {206, 206, 206, 206, 206, 206}, // 62
    {207, 207, 207, 207, 207, 207}, // 63
}};

int TileBlock::face_tile(unsigned face_index) const{
    return tiles[tile_index].get_face_val(face_index);
}

int Tiles::get_face_val(unsigned index) const{
    size_t size = sizeof(*this) / sizeof(int);
    return (index <= size) ? *(reinterpret_cast<const int *>(this) + index) : 0;
}

int Tiles::count_visible_faces() const{
    int count = 0;
    for(int i = 0 ; i < 6 ; i++){
        count += (get_face_val(i) != 0);
    }
    return count;
}

bool TileBlock::is_plant() const{
    switch (tile_index) {
        case BlockType::TALL_GRASS:
        case BlockType::YELLOW_FLOWER:
        case BlockType::RED_FLOWER:
        case BlockType::PURPLE_FLOWER:
        case BlockType::SUN_FLOWER:
        case BlockType::WHITE_FLOWER:
        case BlockType::BLUE_FLOWER:
            return true;
        default:
            return false;
    }
}

bool TileBlock::is_obstacle() const{
    if (is_plant()) {
        return false;
    }
    switch (tile_index) {
        case BlockType::EMPTY:
        case BlockType::CLOUD:
            return false;
        default:
            return true;
    }
}

bool TileBlock::is_transparent() const{
    if (is_plant()) {
        return true;
    }
    switch (tile_index) {
        case BlockType::EMPTY:
        case BlockType::GLASS:
        case BlockType::LEAVES:
            return true;
        default:
            return false;
    }


}

bool TileBlock::is_destructable() const{
    switch (tile_index) {
        case BlockType::EMPTY:
        case BlockType::CLOUD:
            return false;
        default:
            return true;
    }
}

bool TileBlock::is_empty() const {
    return tile_index == BlockType::EMPTY;
}

TileBlock::operator int() const {
    return static_cast<int>(tile_index);
}

BlockType TileBlock::getIndex() const {
    return tile_index;
}

TileBlock::TileBlock(BlockType tile_index) : tile_index{tile_index} {}

bool TileBlock::is_user_buildable() const {
    return is_destructable();
}
