//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_CROSSHAIR_HPP
#define CPP_CROSSHAIR_HPP

#include "RenderableEntity.hpp"
#include "Vertex.hpp"
#include "Shader.hpp"

class Crosshair : public RenderableEntity<Standard2DVertex>{
private:
    using SuperClass = RenderableEntity<Standard2DVertex>;
    static std::vector<Standard2DVertex> get_ends_coordinates(int width, int height, int scale);
public:
    Crosshair(int width, int height, int scale);
    void render_object() = delete;
};


#endif //CPP_CROSSHAIR_HPP
