//
// Created by ultimatenick on 28/07/21.
//
//#include "item.hpp"

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
