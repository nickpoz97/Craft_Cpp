//
// Created by ultimatenick on 11/08/21.
//

#include "glad/glad.h"

#include "vec3.hpp"
#include "OpenglBuffer.hpp"
#include "Vertex.hpp"

template<typename VertexType>
OpenglBuffer<VertexType>::OpenglBuffer(){
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
}

template<typename VertexType>
OpenglBuffer<VertexType>::~OpenglBuffer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

template<typename VertexType>
void OpenglBuffer<VertexType>::draw_triangles() const{
    bind_buffer();

    set_vao_attributes();
    for(int i = 0, j = 0 ; i < attributes_dimensions.size() ; ++i){
        if(attributes_dimensions[i] != 0) {
            glEnableVertexAttribArray(j++);
        };
    }
    glDrawArrays(GL_TRIANGLES, 0, n_indices);
    for(int i = 0, j = 0 ; i < attributes_dimensions.size() ; ++i){
        if(attributes_dimensions[i] != 0) {glDisableVertexAttribArray(j++);};
    }
    unbind_buffer();
}

template<typename VertexType>
void OpenglBuffer<VertexType>::set_vao_attributes() const {
    size_t offset = 0;
    int i = 0;

    for(int a_dim : attributes_dimensions){
        if(a_dim != 0){
            glVertexAttribPointer(i++, a_dim , GL_FLOAT, GL_FALSE, STRIDE, (GLvoid*) offset);
            offset += a_dim * sizeof(float);
        }
    }
}

template<typename VertexType>
void OpenglBuffer<VertexType>::draw_lines() const{
    bind_buffer();
    glVertexAttribPointer(0, get_n_pos_elements<VertexType>(), GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINES, 0, n_indices);
    glDisableVertexAttribArray(0);
    unbind_buffer();
}

template<typename VertexType>
void OpenglBuffer<VertexType>::store_data(const std::vector<VertexType> &buffer) const{
    _store_data(sizeof(buffer), reinterpret_cast<const GLfloat*>(buffer.data()));
}

template<typename VertexType>
void OpenglBuffer<VertexType>::_store_data(int size, const GLfloat *data) const{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

    n_indices = size;
}

template<typename VertexType>
template<size_t n_values>
void OpenglBuffer<VertexType>::store_data(const std::array<VertexType, n_values> &buffer) const {
    _store_data(sizeof(buffer), reinterpret_cast<GLfloat*>(buffer.data()));
}

template<typename VertexType>
void OpenglBuffer<VertexType>::bind_buffer() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
}

template<typename VertexType>
void OpenglBuffer<VertexType>::unbind_buffer() const {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template class OpenglBuffer<Standard2DVertex>;
template class OpenglBuffer<Uv2DVertex>;
template class OpenglBuffer<Standard3DVertex>;
template class OpenglBuffer<NormalVertex>;
template class OpenglBuffer<Uv3DVertex>;
template class OpenglBuffer<CubeVertex>;
