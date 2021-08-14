//
// Created by ultimatenick on 14/08/21.
//

#ifndef CPP_ATTRIB_HPP
#define CPP_ATTRIB_HPP


#include "glad/glad.h"
#include "Shader.hpp"

struct Attrib {
    const GLuint program;
    const GLuint position;
    const GLuint normal;
    const GLuint uv;
    const GLuint matrix;
    const GLuint sampler;
    const GLuint camera;
    const GLuint timer;
    const GLuint extra1;
    const GLuint extra2;
    const GLuint extra3;
    const GLuint extra4;

    Attrib(const Shader& program);
};

#endif //CPP_ATTRIB_HPP
