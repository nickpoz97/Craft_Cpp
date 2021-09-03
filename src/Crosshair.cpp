//
// Created by ultimatenick on 11/08/21.
//

#include "Crosshair.hpp"
#include "Shader.hpp"

Crosshair::Crosshair(const Model &model) : SuperClass(get_ends_coordinates(model)) {};

std::array<Standard2DVertex, 4> Crosshair::get_ends_coordinates(const Model& model) {
    glm::ivec2 position{model.getWidth() / 2, model.getHeight() / 2};
    int p{10 * model.getScale()};

    return {{glm::ivec2{position.x, position.y - p},
             glm::ivec2{position.x, position.y + p},
             glm::ivec2{position.x - p, position.y},
             glm::ivec2{position.x + p, position.y}
    }};
}
