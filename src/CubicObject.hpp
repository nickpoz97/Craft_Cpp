//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_CUBICOBJECT_HPP
#define CPP_CUBICOBJECT_HPP

#include <vector>
#include <vec3.hpp>
#include <mat4x4.hpp>

#include "Vertex.hpp"

class CubicObject {
private:
    static glm::vec3 normals[6];
protected:
    static constexpr float n = 0.5;                 // distance from center

    std::vector<std::vector<CubeVertex>> vertices;
public:
    CubicObject(int n_faces, int n_vertices_face);
    void apply_transform(const glm::mat4 &transform);
};

#endif //CPP_CUBICOBJECT_HPP
