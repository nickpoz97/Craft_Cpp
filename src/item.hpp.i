//
// Created by ultimatenick on 28/07/21.
//
//#include "item.hpp"

template<ItemName I>
const Block Item<I>::blockTypes[]{
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
        {0, 0, 0, 0, 0, 0}, // 17
        {0, 0, 0, 0, 0, 0}, // 18
        {0, 0, 0, 0, 0, 0}, // 19
        {0, 0, 0, 0, 0, 0}, // 20
        {0, 0, 0, 0, 0, 0}, // 21
        {0, 0, 0, 0, 0, 0}, // 22
        {0, 0, 0, 0, 0, 0}, // 23
        /*{0, 0, 0, 0, 0, 0}, // 24
        {0, 0, 0, 0, 0, 0}, // 25
        {0, 0, 0, 0, 0, 0}, // 26
        {0, 0, 0, 0, 0, 0}, // 27
        {0, 0, 0, 0, 0, 0}, // 28
        {0, 0, 0, 0, 0, 0}, // 29
        {0, 0, 0, 0, 0, 0}, // 30
        {0, 0, 0, 0, 0, 0}, // 31*/
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
};

template <ItemName I>
Item<I>::Item() : blockTypeIndex{static_cast<int>(I)} {}

template <ItemName I>
bool Item<I>::is_plant(){
    switch (I) {
        case ItemName::TALL_GRASS:
        case ItemName::YELLOW_FLOWER:
        case ItemName::RED_FLOWER:
        case ItemName::PURPLE_FLOWER:
        case ItemName::SUN_FLOWER:
        case ItemName::WHITE_FLOWER:
        case ItemName::BLUE_FLOWER:
            return true;
        default:
            return false;
    }
}

template <ItemName I>
bool Item<I>::is_obstacle() {
    if (is_plant()) {
        return 1;
    }
    switch (I) {
        case ItemName::EMPTY:
        case ItemName::GLASS:
        case ItemName::LEAVES:
            return 1;
        default:
            return 0;
    }
}

template <ItemName I>
bool Item<I>::is_transparent() {
    if (is_plant()) {
        return 1;
    }
    switch (I) {
        case ItemName::EMPTY:
        case ItemName::GLASS:
        case ItemName::LEAVES:
            return 1;
        default:
            return 0;
    }


}

template<ItemName I>
bool Item<I>::is_destructable() {
    switch (I) {
        case ItemName::EMPTY:
        case ItemName::CLOUD:
            return 0;
        default:
            return 1;
    }
}
