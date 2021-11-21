//
// Created by ultimatenick on 21/11/21.
//

#include "Geometry/Text2D.hpp"
#include "catch2/catch.hpp"

TEST_CASE("Text2D instantiation") {
  glm::ivec2 position{10, 10};
  CraftCpp::Text2D text{position, 2, "Testing Text2D class"};
  REQUIRE(text.initialCharPos == position);
}