//
// Created by ultimatenick on 11/08/21.
//

#include "Crosshair.hpp"
#include "Shader.hpp"

Crosshair::Crosshair(const Model &model) :
    position{model.getWidth() / 2, model.getHeight() / 2},
    p{10 * model.getScale()},
    ends_coordinates{{{position.x, position.y - p},
                      {position.x, position.y + p},
                     {position.x - p, position.y},
                     {position.x + p, position.y}
    }}{
    buffer.store_data(ends_coordinates));
}

Crosshair::~Crosshair() {
    shader.del_buffer()
}
