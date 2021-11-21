//
// Created by ultimatenick on 30/10/21.
//

#include "Rendering/OpenglBuffer.hpp"
#include "catch2/catch.hpp"

TEST_CASE("Instantiate buffer without opengl and glfw initialization") {
  CraftCpp::OpenglBuffer<CraftCpp::Standard3DVertex> buffer{};
  buffer.drawTriangles();
  buffer.storeData({});
}