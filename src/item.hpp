//
// Created by ultimatenick on 28/07/21.
//

#ifndef ITEM_HPP
#define ITEM_HPP

enum class Face{LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK};

// TODO verificare ItemType mancanti
enum class ItemName{
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

class Item{
private:
    ItemName w;

public:
    explicit Item(int index);
    Item() = default;

    bool is_plant() const;
    bool is_obstacle() const;
    bool is_transparent() const;
    bool is_destructable() const;
};

bool Item::is_plant() const{
    switch (w) {
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

bool Item::is_obstacle() const{
    if (is_plant()) {
        return 1;
    }
    switch (w) {
        case ItemName::EMPTY:
        case ItemName::GLASS:
        case ItemName::LEAVES:
            return 1;
        default:
            return 0;
    }
}

bool Item::is_transparent() const{
    if (is_plant()) {
        return 1;
    }
    switch (w) {
        case ItemName::EMPTY:
        case ItemName::GLASS:
        case ItemName::LEAVES:
            return 1;
        default:
            return 0;
    }


}

bool Item::is_destructable() const{
    switch (w) {
        case ItemName::EMPTY:
        case ItemName::CLOUD:
            return 0;
        default:
            return 1;
    }
}

Item::Item(int index) : w{static_cast<ItemName>(index)}{}


#endif
