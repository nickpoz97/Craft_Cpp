//
// Created by ultimatenick on 11/08/21.
//

#include <gtx/fast_exponential.inl>
#include "OpenglBuffer.hpp"
#include "glad/glad.h"
#include "StandardVertex.hpp"

template<typename VertexType>
OpenglBuffer<VertexType>::OpenglBuffer() {
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
void OpenglBuffer<VertexType>::draw_triangles(const Attrib &attrib, int count) const{
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
void OpenglBuffer<CubeVertex>::set_vao_attributes(const Attrib &attrib) const {
    glVertexAttribPointer(attrib.position, 3, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 10, 0);
    glVertexAttribPointer(attrib.normal, 3, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 10, (GLvoid *)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(attrib.uv, 4, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 10, (GLvoid *)(sizeof(GLfloat) * 6));
}

template<>
void OpenglBuffer<UvVertex>::set_vao_attributes(const Attrib &attrib) const{
    glVertexAttribPointer(attrib.position, 3, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 5, 0);
    glVertexAttribPointer(attrib.uv, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 5, (GLvoid *)(sizeof(GLfloat) * 3));
}

template<>
void OpenglBuffer<NormalVertex>::set_vao_attributes(const Attrib &attrib) const{
    glVertexAttribPointer(attrib.position, 3, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 8, 0);
    glVertexAttribPointer(attrib.normal, 3, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 8, (GLvoid *)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(attrib.uv, 2, GL_FLOAT, GL_FALSE,
                          sizeof(GLfloat) * 8, (GLvoid *)(sizeof(GLfloat) * 6));
}

template<typename VertexType>
void OpenglBuffer<VertexType>::draw_lines(const Attrib &attrib, int components, int count) const{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glEnableVertexAttribArray(attrib.position);
    glVertexAttribPointer(
            attrib.position, components, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, count);
    glDisableVertexAttribArray(attrib.position);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
