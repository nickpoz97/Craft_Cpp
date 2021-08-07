//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_GAMEOBJECT_HPP
#define CPP_GAMEOBJECT_HPP

#include <vector>

#include "data_structures.hpp"
#include "Matrix.hpp"

class GameObject {
private:
    static Vec<float,3> normals[6];
protected:
    std::vector<std::vector<Vertex>> vertices;
public:
    GameObject(int n_faces, int n_vertices_face);
    void apply_transform(const Matrix &transform);
};


#endif //CPP_GAMEOBJECT_HPP
