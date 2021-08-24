//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_CROSSHAIR_HPP
#define CPP_CROSSHAIR_HPP


#include "Model.hpp"
#include "Shader.hpp"

class Crosshair {
private:
    glm::ivec2 position;
    int p;
    std::array<glm::vec2, 4> ends_coordinates;
    OpenglBuffer<Standard2DVertex> buffer;

public:
    Crosshair(const Model& model);
    ~Crosshair();
};


#endif //CPP_CROSSHAIR_HPP
