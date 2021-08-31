//
// Created by ultimatenick on 11/08/21.
//

#include <fstream>
#include <sstream>
#include "Shader.hpp"

Shader::Shader(std::string_view vs_path, std::string_view fs_path, const UniformWrapper &uniforms) : uniforms{uniforms} {
    unsigned vs_id = build_shader(vs_path);
    unsigned fs_id = build_shader(fs_path);

    id = glCreateProgram();
    glAttachShader(id, vs_id);
    glAttachShader(id, fs_id);
    glLinkProgram(id);
    glDeleteProgram(vs_id);
    glDeleteProgram(fs_id);
}

GLuint Shader::get_id() const {
    return id;
}

void Shader::use() {
    glUseProgram(id);
}

int Shader::build_shader(std::string_view path) {
    std::ifstream code_file;
    std::stringstream code_stream;

    code_file.open(path.data());
    code_stream << code_file.rdbuf();
    code_file.close();
    std::string_view code_string{code_stream.str()};

    const char* code_c_string = code_string.data();
    unsigned id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, &(code_c_string), NULL);
    glCompileShader(id);
}
