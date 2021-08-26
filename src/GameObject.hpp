//
// Created by ultimatenick on 12/08/21.
//

#ifndef CPP_GAMEOBJECT_HPP
#define CPP_GAMEOBJECT_HPP


#include <vector>
#include "vec3.hpp"
#include "mat4x4.hpp"
#include "OpenglBuffer.hpp"
#include "TileBlock.hpp"

using vertex_iterator = std::vector<CubeVertex>::iterator;

template<typename VertexType>
class GameObject{
private:
    glm::mat4 transform_matrix{1.0};
protected:
    vertex_iterator begin;
    vertex_iterator end;
public:
    vertex_iterator get_end() const;

public:
    explicit GameObject(const vertex_iterator& begin_it, int n_vertices);
};

template<typename VertexType>
GameObject<VertexType>::GameObject(const vertex_iterator& begin_it, int n_vertices) : begin{begin_it}, end{begin_it + n_vertices}{}

template<typename VertexType>
vertex_iterator GameObject<VertexType>::get_end() const {
    return end;
}

#endif //CPP_GAMEOBJECT_HPP
