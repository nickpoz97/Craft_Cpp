//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_CROSSHAIR_HPP
#define CPP_CROSSHAIR_HPP


#include "Model.hpp"
#include "Shader.hpp"

class Crosshair : public GameObject<Standard2DVertex>{
private:
    using SuperClass = GameObject<Standard2DVertex>;
    static std::array<Standard2DVertex, 4> get_ends_coordinates(const Model& model);
public:
    Crosshair(const Model& model);
    void render_object() = delete;
};


#endif //CPP_CROSSHAIR_HPP
