//
// Created by ultimatenick on 27/09/21.
//
#include "catch.hpp"
#include "Geometry/TileBlock.hpp"

TEST_CASE("Tileblock instantiation", "[instantiation]"){
    int i = -1;
    BlockType w;
    do{
        w = static_cast<BlockType>(++i);
        TileBlock t{w};
        REQUIRE(t.get_index() == w);
        auto other_it = TileBlock::tiles[i].begin(t.is_plant());
        for(auto tile : t){
            REQUIRE(tile == *other_it);
            ++other_it;
        }
    }
    while(w != BlockType::COLOR_31);
}
