//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_VERTEX_HPP
#define CPP_VERTEX_HPP

#include <vec4.hpp>
#include <vec2.hpp>

enum class Face{LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK};

struct Standard2DVertex{
    glm::vec2 position;
};

struct Uv2DVertex : Standard2DVertex{
    glm::vec2 uv;
};

struct Standard3DVertex{
    glm::vec3 position;
};

struct Uv3DVertex : Standard3DVertex{
    glm::vec2 uv;
};

struct NormalVertex : Uv3DVertex{
    glm::vec3 normal;
};

class CubeVertex : public NormalVertex{
public:
    glm::vec2 ao_light;
    /*Face face;
    int index;*/
};



#endif //CPP_VERTEX_HPP
