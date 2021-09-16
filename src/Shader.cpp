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

Shader::Shader(std::string_view vs_path,
               std::string_view fs_path){
    unsigned vs_id = build_shader(vs_path);
    unsigned fs_id = build_shader(fs_path);

    id = glCreateProgram();
    glAttachShader(id, vs_id);
    glAttachShader(id, fs_id);
    glLinkProgram(id);
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

int Shader::build_shader(std::string_view path) {
    std::ifstream code_file;
    std::stringstream code_stream;

    code_file.open(path.data());
    code_stream << code_file.rdbuf();
    code_file.close();
    std::string_view code_string{code_stream.str()};

    const char* code_c_string = code_string.data();

    unsigned shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_id, 1, &(code_c_string), NULL);
    glCompileShader(shader_id);

    return shader_id;
}

void Shader::get_uniforms_location() {
    for(auto& u : uniforms){
        u.id = glGetUniformLocation(id, u.name.data());
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