//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_STANDARDVERTEX_HPP
#define CPP_STANDARDVERTEX_HPP

#include <vec4.hpp>
#include <vec2.hpp>

enum class Face{LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK};

struct StandardVertex{
    glm::vec4 position;
};

struct UvVertex : StandardVertex{
    glm::vec2 uv;
};

class CubeVertex : public UvVertex{
public:
    float ao;
    float light;
    Face face;
    int index;
};



#endif //CPP_STANDARDVERTEX_HPP
