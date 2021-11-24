//
// Created by ultimatenick on 11/08/21.
//

#include "Geometry/Crosshair.hpp"
#include "Geometry/Vertex.hpp"
#include "Rendering/Shader.hpp"
#include <iostream>

namespace CraftCpp {
Crosshair::Crosshair(int width, int height, int scale)
    : SuperClass(getEndsCoordinates(width, height, scale)), centerPos{width / 2,
                                                                      height /
                                                                          2} {}

std::vector<Standard2DVertex>
Crosshair::getEndsCoordinates(int width, int height, int scale) {
  glm::ivec2 position{width / 2, height / 2};
  int p{10 * scale};

  return {Standard2DVertex{{position.x, position.y - p}},
          Standard2DVertex{{position.x, position.y + p}},
          Standard2DVertex{{position.x - p, position.y}},
          Standard2DVertex{{position.x + p, position.y}}};
}

void Crosshair::renderLines() const {
  glLogicOp(GL_INVERT);
  glEnable(GL_COLOR_LOGIC_OP);
  SuperClass::renderLines();
  glDisable(GL_COLOR_LOGIC_OP);
}
} // namespace CraftCpp