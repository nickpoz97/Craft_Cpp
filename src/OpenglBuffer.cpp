//
// Created by ultimatenick on 11/08/21.
//

#include "OpenglBuffer.hpp"
#include "glad/glad.h"

OpenglBuffer::OpenglBuffer() {
    glGenBuffers(1, &id);
}

void OpenglBuffer::store_data(GLsizei size, const GLfloat* const data) const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
}

OpenglBuffer::~OpenglBuffer() {
    glDeleteBuffers(1, &id);
}
