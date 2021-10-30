//
// Created by ultimatenick on 27/09/21.
//
#include "catch2/catch.hpp"
#include "Geometry/TileBlock.hpp"

TEST_CASE("Tileblock instantiation", "[instantiation]"){
    int i = -1;
    CraftCpp::BlockType w;
    do{
        w = static_cast<CraftCpp::BlockType>(++i);
        CraftCpp::TileBlock t{w};
        REQUIRE(t.getBlockType() == w);
        auto other_it = CraftCpp::TileBlock::tiles[i].begin();
        for(auto tile : t){
            REQUIRE(tile == *other_it);
            ++other_it;
        }
    }
    while(w != CraftCpp::BlockType::COLOR_31);
}
