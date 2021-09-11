//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_VERTEX_HPP
#define CPP_VERTEX_HPP

#include "vec4.hpp"
#include "vec2.hpp"
#include "mat4x4.hpp"

struct Standard2DVertex{
    glm::vec2 position;

    operator glm::ivec2();
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
    //glm::mat4 transform;
    //glm::vec2 ao_light;
};



#endif //CPP_VERTEX_HPP
