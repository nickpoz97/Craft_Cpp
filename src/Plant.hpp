//
// Created by ultimatenick on 06/08/21.
//

#ifndef CPP_PLANT_HPP
#define CPP_PLANT_HPP

#include <array>
#include <vector>

#include "item.hpp"
#include "CubicObject.hpp"

class Plant : public CubicObject<4, INDICES_FACE_COUNT>{
private:
    static const glm::vec3 local_vertex_positions[4][4];
    //static const glm::vec3 normals[4];
    static const glm::vec2 uvs[4][4];
    static const float indices[4][6];

    static constexpr float s = 0.0625;
    static constexpr float a = 0;
    static constexpr float b = s;

    static constexpr int n_faces = 4;
    static constexpr int n_vertices_face = 6;
public:
    Plant(const glm::vec3& center_position, int tileIndex, float ao, float light);
};


#endif //CPP_PLANT_HPP
