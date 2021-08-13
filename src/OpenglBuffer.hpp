//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_OPENGLBUFFER_HPP
#define CPP_OPENGLBUFFER_HPP

#include "glad/glad.h"

class OpenglBuffer {
private:
    GLuint id;

public:
    OpenglBuffer();
    ~OpenglBuffer();

    void store_data(GLsizei size, const GLfloat* const data) const;
};


#endif //CPP_OPENGLBUFFER_HPP
