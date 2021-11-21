//
// Created by ultimatenick on 01/11/21.
//

#include "Rendering/Shader.hpp"
#include "catch2/catch.hpp"
#include <Interaction/GameView.hpp>

TEST_CASE(
    "Shader instantiation without GameView initialized (no opengl context)") {
  CraftCpp::Shader s("foo1", "foo2");
  s.use();
  REQUIRE(s.getId() == 0);
}

TEST_CASE("Shader instantiation with GameView initialized (opengl context "
          "available)") {
  auto gmvRef = CraftCpp::GameView::setInstance(1, 1, 1);
  CraftCpp::Shader s("../data/shaders/block_vertex.glsl",
                     "../data/shaders/block_fragment.glsl");
  REQUIRE(s.getId() != 0);
}