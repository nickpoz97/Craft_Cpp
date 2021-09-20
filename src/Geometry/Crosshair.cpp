//
// Created by ultimatenick on 11/08/21.
//

#include <iostream>
#include "Crosshair.hpp"
#include "../Rendering/Shader.hpp"
#include "Vertex.hpp"

Crosshair::Crosshair(int width, int height, int scale) : SuperClass(get_ends_coordinates(width, height, scale)) {};

std::vector<Standard2DVertex> Crosshair::get_ends_coordinates(int width, int height, int scale) {
    glm::ivec2 position{width / 2, height / 2};
    int p{10 * scale};

    Standard2DVertex st{{2,3}};
    std::cerr << "updated crosshair position: \n" << position.x << ", " << position.y << '\n';

    return {
        Standard2DVertex{{position.x, position.y - p}},
        Standard2DVertex{{position.x, position.y + p}},
        Standard2DVertex{{position.x - p, position.y}},
        Standard2DVertex{{position.x + p, position.y}}
    };
}

void Crosshair::update(int width, int height, int scale) {
    SuperClass::update_buffer(get_ends_coordinates(width, height, scale));
}
