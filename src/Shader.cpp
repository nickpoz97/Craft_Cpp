//
// Created by ultimatenick on 11/08/21.
//

#include <fstream>
#include <sstream>
#include <vec3.hpp>
#include <gtc/type_ptr.hpp>
#include "Shader.hpp"

Shader::Shader(std::string_view vs_path,
               std::string_view fs_path,
               std::array<std::string_view, 4> uniforms_extra_names) :
    uniforms{uniforms_extra_names}{
    unsigned vs_id = build_shader(vs_path);
    unsigned fs_id = build_shader(fs_path);

    id = glCreateProgram();
    glAttachShader(id, vs_id);
    glAttachShader(id, fs_id);
    glLinkProgram(id);
    glDeleteProgram(vs_id);
    glDeleteProgram(fs_id);

    get_uniforms_location();
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
        u.first = glGetUniformLocation(id, u.second.data());
    }
}

void Shader::set_camera(const glm::vec3 &camera_pos) const {
    glUniform3fv(static_cast<GLint>(uniforms.camera.first), 1, glm::value_ptr(camera_pos));
}

void Shader::set_sampler(int sampler) const{
    glUniform1i(static_cast<GLint>(uniforms.sampler.first), sampler);
}

void Shader::set_timer(float timer) const{
    glUniform1i(static_cast<GLint>(uniforms.timer.first), timer);
}

template<typename GLtype>
void Shader::set_extra(int extra_suffix, GLtype value) const{
    if(extra_suffix < 1 || extra_suffix > 4){
        return;
    }
    switch (extra_suffix) {
        case 1:
            _set_extra(uniforms.extra1.first, value);
        break;
        case 2:
            _set_extra(uniforms.extra2.first, value);
        break;
        case 3:
            _set_extra(uniforms.extra3.first, value);
        break;
        case 4:
            _set_extra(uniforms.extra4.first, value);
        break;
    }
}

void Shader::set_viewproj(const glm::mat4& m) const{
    glUniformMatrix4fv(uniforms.viewproj_matrix.first, 1, GL_FALSE, glm::value_ptr(m));
}

template<>
void Shader::_set_extra<float>(int u_location, float val) const{
    glUniform1f(u_location, val);
}

template<>
void Shader::_set_extra<int>(int u_location, int val) const{
    glUniform1i(u_location, val);
}

template void Shader::set_extra(int extra_suffix, float value) const;
template void Shader::set_extra(int extra_suffix, int value) const;