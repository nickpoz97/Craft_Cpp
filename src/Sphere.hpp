//
// Created by ultimatenick on 08/08/21.
//

#ifndef CPP_SPHERE_HPP
#define CPP_SPHERE_HPP

#include <list>

#include "CubicObject.hpp"

//TODO update mathematics
class Sphere{
private:
    static constexpr int N_TRIANGLES = 8;

    static std::array<std::array<int,3>,N_TRIANGLES> triangles_indices;
    static std::array<Vec<float,3>,6> positions;
    static std::array<Vec<float,2>,6> uvs;
    static constexpr float PI = static_cast<float>(3.14159265359);

    std::list<Vertex> vertices;
    const float r;

    void rec_gen_sphere(int detail, const std::array<Vec<float, 3>, 3> &actual_pos, const std::array<Vec<float, 2>, 3> &actual_uvs);
    void store_vertices(const std::array<Vec<float, 3>, 3> &actual_pos, const std::array<Vec<float, 2>, 3> &actual_uvs);

public:
    Sphere(float r, int detail);
};


#endif //CPP_SPHERE_HPP
