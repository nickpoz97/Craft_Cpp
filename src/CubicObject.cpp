//
// Created by ultimatenick on 07/08/21.
//

#include <vec3.hpp>
#include "CubicObject.hpp"

glm::vec3 CubicObject::normals[6]{
    {-1, 0, 0},
    {+1, 0, 0},
    {0, +1, 0},
    {0, -1, 0},
    {0, 0, -1},
    {0, 0, +1}
};

CubicObject::CubicObject(int n_faces, int n_vertices_face) :  vertices(n_faces, std::vector<CubeVertex>(n_vertices_face)){
}

void CubicObject::apply_transform(const glm::mat4 &transform) {
    for(auto& face : vertices){
        for(auto& v : face){
            v.position = transform * v.position;
        }
    }
}
