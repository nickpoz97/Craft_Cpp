//
// Created by ultimatenick on 21/11/21.
//

#include "Geometry/Crosshair.hpp"
#include "catch2/catch.hpp"

TEST_CASE("Crosshair instantiation") {
  int width{10}, height{4}, scale{2};

  CraftCpp::Crosshair c{width, height, scale};
  REQUIRE(c.centerPos == glm::ivec2{width / 2, height / 2});
}