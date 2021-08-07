//
// Created by ultimatenick on 29/07/21.
//

#ifndef CPP_CUBE_HPP
#define CPP_CUBE_HPP

#include <array>

#include "item.hpp"
#include "data_structures.hpp"
#include "GameObject.hpp"

class Cube : public GameObject{
private:
    static Vec<float, 3> local_vertex_positions[6][4];
    static Vec<bool, 2> uvs[6][4];
    static int indices[6][6];
    static int flipped[6][6];

    static constexpr float n = 0.5;                 // distance from center
    static constexpr float s = 0.0625;              // TODO check semantic
    static constexpr float a = 0.0 + 1 / 2048.0;    // TODO check semantic
    static constexpr float b = s - 1 / 2048.0;      // TODO check semantic

    static constexpr int n_faces = 6;
    static constexpr int n_vertices_face = 6;

public:
    Cube(const Vec<float, 3> &center_position, int tileIndex, float ao[6][4], float light[6][4]);
};


#endif //CPP_CUBE_HPP
