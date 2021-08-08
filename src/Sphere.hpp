//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_SPHERE_HPP
#define CPP_SPHERE_HPP


#include "GameObject.hpp"

class Sphere{
private:
    static constexpr int N_TRIANGLES = 8;

    static std::array<std::array<int,3>,N_TRIANGLES> indices;
    static std::array<Vec<float,3>,6> positions;
    static std::array<Vec<float,2>,6> uvs;

    std::vector<Vertex> vertices;

public:
    Sphere(int r, int detail);
};


#endif //CPP_SPHERE_HPP
