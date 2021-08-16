//
// Created by ultimatenick on 11/08/21.
//

#include <vec3.hpp>

#include "OpenglBuffer.hpp"
#include "glad/glad.h"
#include "Vertex.hpp"

template<typename VertexType>
OpenglBuffer<VertexType>::OpenglBuffer(const Shader& program){
    glGenBuffers(1, &id);
}

template<typename VertexType>
void OpenglBuffer<VertexType>::store_data(GLsizei size, const GLfloat* const data) const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
}

template<typename VertexType>
OpenglBuffer<VertexType>::~OpenglBuffer() {
    glDeleteBuffers(1, &id);
}

template<typename VertexType>
void OpenglBuffer<VertexType>::draw_triangles(int count) const{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glEnableVertexAttribArray(attrib.position);
    glEnableVertexAttribArray(attrib.normal);
    glEnableVertexAttribArray(attrib.uv);

    set_vao_attributes(attrib);

    glDrawArrays(GL_TRIANGLES, 0, count);
    glDisableVertexAttribArray(attrib.position);
    glDisableVertexAttribArray(attrib.normal);
    glDisableVertexAttribArray(attrib.uv);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//TODO check size of stride and position

template<>
void OpenglBuffer<CubeVertex>::set_vao_attributes() const {
    glVertexAttribPointer(attrib.position, N_POS_ELEMENTS<CubeVertex>::v , GL_FLOAT, GL_FALSE,
                          STRIDE, (GLvoid *) offsetof(CubeVertex, position));
    glVertexAttribPointer(attrib.uv, N_UV_ELEMENTS<CubeVertex>::v, GL_FLOAT, GL_FALSE,
                          STRIDE, (GLvoid *) offsetof(CubeVertex, uv));
    glVertexAttribPointer(attrib.normal, N_NORMAL_ELEMENTS<CubeVertex>::v, GL_FLOAT, GL_FALSE,
                          STRIDE, (GLvoid *) offsetof(CubeVertex, normal));
}

template<>
void OpenglBuffer<Uv3DVertex>::set_vao_attributes() const{
    glVertexAttribPointer(attrib.position, N_POS_ELEMENTS<Uv3DVertex>::v, GL_FLOAT, GL_FALSE,
                          STRIDE, (GLvoid *) offsetof(CubeVertex, position));
    glVertexAttribPointer(attrib.uv, N_UV_ELEMENTS<Uv3DVertex>::v, GL_FLOAT, GL_FALSE,
                          STRIDE, (GLvoid *) offsetof(CubeVertex, uv));
}

template<>
void OpenglBuffer<NormalVertex>::set_vao_attributes() const{
    dynamic_cast<const OpenglBuffer<CubeVertex>&>(*this).set_vao_attributes();
}

template<typename VertexType>
void OpenglBuffer<VertexType>::draw_lines(int components, int count) const{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glEnableVertexAttribArray(attrib.position);
    glVertexAttribPointer(
            attrib.position, components, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, count);
    glDisableVertexAttribArray(attrib.position);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
