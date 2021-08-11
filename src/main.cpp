#include <iostream>
#include "item.hpp"

int main() {
    Item<ItemName::GRASS> grass;
    std::cout << "Hello, World!" << std::endl;

    TileBlock b = {1, 2, 3, 4, 5, 6};
    std::cout << b[5] << std::endl;
    return 0;
}
