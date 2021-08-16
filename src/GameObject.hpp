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
    const OpenglBuffer<VertexType> buffer;
protected:
    std::vector<VertexType> vertices;
    const Shader& shader;
public:
    GameObject(const Shader& shader);
    void set_buffer_data() const;
    void apply_transform(const glm::mat4 &transform);
    const OpenglBuffer<VertexType> &getBuffer() const;
    void draw_triangles();
    void draw_lines();
};

// TODO take ownership of vertices array that will be passed to constructor
template<typename VertexType, int n_vertices>
GameObject<VertexType, n_vertices>::GameObject(const Shader& shader) : vertices(n_vertices), buffer(), shader(shader) {}

template<typename VertexType, int n_vertices>
void GameObject<VertexType, n_vertices>::set_buffer_data() const {
    buffer.store_data(vertices.size() * sizeof(VertexType), reinterpret_cast<const GLfloat* const>(vertices.data()));
}

template<typename VertexType, int n_vertices>
void GameObject<VertexType, n_vertices>::apply_transform(const glm::mat4 &transform) {
    for(auto& v : this->vertices){
        v.position = transform * glm::vec4{v.position, 1.0f};
    }
}

template<typename VertexType, int n_vertices>
const OpenglBuffer<VertexType> &GameObject<VertexType, n_vertices>::getBuffer() const {
    return buffer;
}

template<typename VertexType, int n_vertices>
void GameObject<VertexType, n_vertices>::draw_triangles() {
    buffer.draw_triangles(shader.getAttrib(), vertices.size());
}

template<typename VertexType, int n_vertices>
void GameObject<VertexType, n_vertices>::draw_lines() {
    buffer.draw_lines(shader.getAttrib(), vertices.size());
}

#endif //CPP_GAMEOBJECT_HPP
