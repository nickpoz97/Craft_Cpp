//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_OPENGLBUFFER_HPP
#define CPP_OPENGLBUFFER_HPP

#include "glad/glad.h"

class OpenglBuffer {
private:
    GLuint id;

    OpenglBuffer(GLsizei size, GLfloat *data);
    ~OpenglBuffer();
};


#endif //CPP_OPENGLBUFFER_HPP
