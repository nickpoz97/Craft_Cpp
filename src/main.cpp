#include <iostream>
#include "item.hpp"
#include "data_structures.hpp"

int main() {
    Item<ItemName::GRASS> grass;
    std::cout << "Hello, World!" << std::endl;

    Block b = {1,2,3,4,5,6};
    std::cout << b[5] << std::endl;
    return 0;
}
