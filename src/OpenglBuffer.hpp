//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_OPENGLBUFFER_HPP
#define CPP_OPENGLBUFFER_HPP

#include "glad/glad.h"
#include "Attrib.hpp"

template<typename VertexType>
class OpenglBuffer {
private:
    GLuint id;
    void set_vao_attributes(const Attrib &attrib) const;

public:
    OpenglBuffer();
    ~OpenglBuffer();

    void store_data(GLsizei size, const GLfloat* const data) const;
    void draw_triangles(const Attrib& attrib, int count) const;
    void draw_lines(const Attrib &attrib, int components, int count) const ;
};


#endif //CPP_OPENGLBUFFER_HPP
