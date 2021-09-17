//
// Created by ultimatenick on 11/08/21.
//

#define GLFW_INCLUDE_NONE

#include <fstream>
#include <sstream>
#include <iostream>
#include <vec3.hpp>
#include "Shader.hpp"
#include "gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLError.hpp"

Shader::Shader(std::string_view vs_path,
               std::string_view fs_path){
    unsigned vs_id = build_shader(vs_path, shader_type::VERTEX);
    check_compile_errors(vs_id, vs_path.data());
    unsigned fs_id = build_shader(fs_path, shader_type::FRAGMENT);
    check_compile_errors(fs_id, fs_path.data());

    id = glCreateProgram();
    glAttachShader(id, vs_id);
    glAttachShader(id, fs_id);
    glLinkProgram(id);
    check_compile_errors(id, "PROGRAM");

    glDeleteProgram(vs_id);
    glDeleteProgram(fs_id);

    get_uniforms_location();

    // set PI constant
    glUniform1f(static_cast<GLint>(uniforms.PI.id), glm::pi<float>());
}

GLuint Shader::get_id() const {
    return id;
}

void Shader::use() const{
    glUseProgram(id);
}

int Shader::build_shader(std::string_view path, shader_type st) {
    std::ifstream code_file;
    std::stringstream code_stream;

    code_file.open(path.data());
    code_stream << code_file.rdbuf();
    code_file.close();
    std::string code_string{code_stream.str()};

    const char* code_c_string = code_string.data();

    unsigned shader_id;
    switch (st) {
        case shader_type::VERTEX:
            shader_id = glCreateShader(GL_VERTEX_SHADER);
        break;
        case shader_type::FRAGMENT:
            shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
    glShaderSource(shader_id, 1, &(code_c_string), NULL);
    glCompileShader(shader_id);

    return shader_id;
}

void Shader::get_uniforms_location() {
    for(auto& u : uniforms){
        //std::cerr << u.name << " value before: "<< u.id << '\n';
        u.id = glGetUniformLocation(id, u.name.data());
        //std::cerr << u.name << " value after: "<< u.id << '\n';
    }
}

void Shader::set_camera(const glm::vec3 &camera_pos) const {
    glUniform3fv(static_cast<GLint>(uniforms.camera.id), 1, glm::value_ptr(camera_pos));
}

void Shader::set_sampler(int sampler) const{
    glUniform1i(static_cast<GLint>(uniforms.sampler.id), sampler);
}

void Shader::set_timer(float timer) const{
    glUniform1f(static_cast<GLint>(uniforms.timer.id), timer);
}

template<typename GLtype>
void Shader::set_extra_uniform(std::string_view uniform_name, GLtype value) const{
    _set_extra_uniform(glGetUniformLocation(id, uniform_name.data()), value);
}

void Shader::set_viewproj(const glm::mat4& m) const{
    glUniformMatrix4fv(uniforms.viewproj_matrix.id, 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::check_compile_errors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

template<>
void Shader::_set_extra_uniform<float>(int u_location, float val) const{
    glUniform1f(u_location, val);
}

template<>
void Shader::_set_extra_uniform<int>(int u_location, int val) const{
    glUniform1i(u_location, val);
}

template void Shader::set_extra_uniform(std::string_view id, float value) const;
template void Shader::set_extra_uniform(std::string_view id, int value) const;