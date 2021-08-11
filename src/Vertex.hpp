//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_VERTEX_HPP
#define CPP_VERTEX_HPP

#include <vec4.hpp>
#include <vec2.hpp>

enum class Face{LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK};

struct Vertex{
    glm::vec4 position;
    glm::bvec2 uv;
};

class CubeVertex : public Vertex{
public:
    float ao;
    float light;
    Face face;
    int index;
};



#endif //CPP_VERTEX_HPP
