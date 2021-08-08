//
// Created by ultimatenick on 07/08/21.
//

#include "GameObject.hpp"
#include "Matrix.hpp"

Vec<float,3> GameObject::normals[6]{
    {-1, 0, 0},
    {+1, 0, 0},
    {0, +1, 0},
    {0, -1, 0},
    {0, 0, -1},
    {0, 0, +1}
};

GameObject::GameObject(int n_faces, int n_vertices_face) : vertices(n_faces, std::vector<CubeVertex>(n_vertices_face)){

}

void GameObject::apply_transform(const Matrix &transform) {
    for(auto& face : vertices){
        for(auto& v : face){
            v.setPosition(transform * v.getPosition());
        }
    }
}
