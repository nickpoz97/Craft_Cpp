//
// Created by ultimatenick on 11/08/21.
//

#include "Crosshair.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"

Crosshair::Crosshair(int width, int height, int scale) : SuperClass(get_ends_coordinates(width, height, scale)) {};

std::vector<Standard2DVertex> Crosshair::get_ends_coordinates(int width, int height, int scale) {
    glm::ivec2 position{width / 2, height / 2};
    int p{10 * scale};

    Standard2DVertex st{{2,3}};

    return {
        Standard2DVertex{{position.x, position.y - p}},
        Standard2DVertex{{position.x, position.y + p}},
        Standard2DVertex{{position.x - p, position.y}},
        Standard2DVertex{{position.x + p, position.y}}
    };
}
