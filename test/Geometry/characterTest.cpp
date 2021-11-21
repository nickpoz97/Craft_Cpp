//
// Created by ultimatenick on 21/11/21.
//

#include "catch2/catch.hpp"
#include "Geometry/Character.hpp"

TEST_CASE("Character instantiation"){
    glm::ivec2 position{5,6};

    CraftCpp::Character c{position, 2,2, 'a'};
    REQUIRE(c.centerPos == position);
}