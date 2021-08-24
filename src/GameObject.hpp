//
// Created by ultimatenick on 12/08/21.
//

#ifndef CPP_GAMEOBJECT_HPP
#define CPP_GAMEOBJECT_HPP


#include <vector>
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "OpenglBuffer.hpp"
#include "Block.hpp"

template<typename VertexType>
class GameObject{
private:
    glm::mat4 transform_matrix{1.0};
protected:
    static const int n_vertices;
    std::vector<VertexType> vertices;
public:
    explicit GameObject(int n_vertices);
    void apply_transform(const glm::mat4 &tr);
    void apply_transform(const glm::mat4 &&tr);
};

// TODO take ownership of vertices array that will be passed to constructor
template<typename VertexType>
GameObject<VertexType>::GameObject(int n_vertices) : vertices(n_vertices){}

template<typename VertexType>
void GameObject<VertexType>::apply_transform(const glm::mat4 &tr) {
    transform_matrix = tr;
    // TODO do this in shader
}

template<typename VertexType>
void GameObject<VertexType>::apply_transform(const glm::mat4 &&tr) {
    transform_matrix = tr;
    // TODO do this in shader
}

#endif //CPP_GAMEOBJECT_HPP
