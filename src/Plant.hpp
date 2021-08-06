//
// Created by ultimatenick on 06/08/21.
//

#ifndef CPP_PLANT_HPP
#define CPP_PLANT_HPP

#include <array>

#include "data_structures.hpp"
#include "item.hpp"

class Plant {
private:
    static const Vec<float, 3> local_vertex_positions[4][4];
    static const Vec<float,3> normals[4];
    static const Vec<float,2> uvs[4][4];
    static const float indices[4][6];

    static constexpr float s = 0.0625;
    static constexpr float a = 0;
    static constexpr float b = s;
    static constexpr float n = 0.5;                 // distance from center

    static constexpr int n_faces = 4;
    static constexpr int n_vertices_face = 6;
    std::array<std::array<Vertex, n_vertices_face>, n_faces> vertices;
public:
    Plant(const Vec<float, 3>& center_position, int tileIndex, float ao, float light);
};


#endif //CPP_PLANT_HPP
