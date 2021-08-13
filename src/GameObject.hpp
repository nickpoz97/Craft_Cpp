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

template<typename VertexType, int n_vertices>
class GameObject{
private:
    const OpenglBuffer buffer;
protected:
    std::vector<VertexType> vertices;
public:
    GameObject();
    void set_buffer_data() const;
    void apply_transform(const glm::mat4 &transform);
};

template<typename VertexType, int n_vertices>
GameObject<VertexType, n_vertices>::GameObject() : vertices(n_vertices), buffer() {}

template<typename VertexType, int n_vertices>
void GameObject<VertexType, n_vertices>::set_buffer_data() const {
    buffer.store_data(vertices.size() * sizeof(VertexType), reinterpret_cast<const GLfloat* const>(vertices.data()));
}

template<typename VertexType, int n_vertices>
void GameObject<VertexType, n_vertices>::apply_transform(const glm::mat4 &transform) {
    for(auto& v : this->vertices){
        v.position = transform * v.position;
    }
}

#endif //CPP_GAMEOBJECT_HPP
