//
// Created by ultimatenick on 11/08/21.
//

#include <iostream>
#include "Geometry/Crosshair.hpp"
#include "Geometry/Vertex.hpp"
#include "Rendering/Shader.hpp"

namespace CraftCpp {
Crosshair::Crosshair(int width, int height, int scale) : SuperClass(getEndsCoordinates(width, height, scale)) {};

std::vector<Standard2DVertex> Crosshair::getEndsCoordinates(int width, int height, int scale) {
    glm::ivec2 position{width / 2, height / 2};
    int p{10 * scale};

    Standard2DVertex st{{2, 3}};
    std::cerr << "updated crosshair position: \n" << position.x << ", " << position.y << '\n';

    return {
            Standard2DVertex{{position.x, position.y - p}},
            Standard2DVertex{{position.x, position.y + p}},
            Standard2DVertex{{position.x - p, position.y}},
            Standard2DVertex{{position.x + p, position.y}}
    };
}

void Crosshair::renderLines() const {
    glLogicOp(GL_INVERT);
    glEnable(GL_COLOR_LOGIC_OP);
    SuperClass::renderLines();
    glDisable(GL_COLOR_LOGIC_OP);
}
}