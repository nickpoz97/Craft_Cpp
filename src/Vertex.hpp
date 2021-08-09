//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_VERTEX_HPP
#define CPP_VERTEX_HPP

#include "data_structures.hpp"

enum class Face{LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK};

struct Vertex{
    Vec<float, 4> position;
    Vec<float,2> uv;
};

class CubeVertex : public Vertex{
public:
    Face getFace() const;
    void setFace(Face face);
    float getAo() const;
    void setAo(float ao);
    float getLight() const;
    void setLight(float light);

private:
    float ao;
    float light;
    Face face;
    int index;
public:
    int getIndex() const;

    void setIndex(int index);
};



#endif //CPP_VERTEX_HPP
