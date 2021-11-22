//
// Created by ultimatenick on 11/08/21.
//

#include <Interaction/GameView.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Rendering/OpenGlExternal.hpp"
#include "Rendering/Shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec3.hpp"

namespace CraftCpp {
Shader::Shader(std::string_view vs_path, std::string_view fs_path) {
  if (!GameView::isInstantiated()) {
    return;
  }

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
}

GLuint Shader::getId() const { return id; }

void Shader::use() const {
  if (id != 0) {
    glUseProgram(id);
  }
#ifndef NDEBUG
  else {
    std::cerr << "Shader not ready (check GameView status)\n";
  }
#endif
}

uint Shader::build_shader(std::string_view path, shader_type st) {
  std::ifstream code_file;
  std::stringstream code_stream;

  code_file.open(path.data());
  code_stream << code_file.rdbuf();
  code_file.close();
  std::string code_string{code_stream.str()};

  const char *code_c_string = code_string.data();

  unsigned shader_id;
  switch (st) {
  case shader_type::VERTEX:
    shader_id = glCreateShader(GL_VERTEX_SHADER);
    break;
  case shader_type::FRAGMENT:
    shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    break;
  }
  glShaderSource(shader_id, 1, &(code_c_string), nullptr);
  glCompileShader(shader_id);

  return shader_id;
}

void Shader::setCamera(const glm::vec3 &camera_pos) const {
  setExtraUniform("camera_pos", camera_pos);
}

void Shader::setSampler(int sampler) const {
  setExtraUniform("sampler", sampler);
}

void Shader::setTimer(float timer) const { setExtraUniform("timer", timer); }

template <typename GLtype>
void Shader::setExtraUniform(std::string_view uniform_name,
                             const GLtype &value) const {
  if (id != 0) {
    use();
    _setExtraUniform(glGetUniformLocation(id, uniform_name.data()), value);
  }
}

void Shader::setViewProj(const glm::mat4 &m) const {
  setExtraUniform("viewproj_matrix", m);
}

void Shader::check_compile_errors(GLuint shader, const std::string &type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
      std::cout
          << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
      std::cout
          << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  }
}

void Shader::_setExtraUniform(int u_location, float val) {
  glUniform1f(u_location, val);
}

void Shader::_setExtraUniform(int u_location, int val) {
  glUniform1i(u_location, val);
}

void Shader::_setExtraUniform(int u_location, const glm::vec3 &val) {
  glUniform3fv(u_location, 1, glm::value_ptr(val));
}

void Shader::_setExtraUniform(int u_location, const glm::mat4 &val) {
  glUniformMatrix4fv(u_location, 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::_setExtraUniform(int u_location, const glm::vec2 &val) {
  glUniform2fv(u_location, 1, glm::value_ptr(val));
}

void Shader::set_pi() const { setExtraUniform("PI", glm::pi<float>()); }

template void Shader::setExtraUniform<float>(std::string_view id,
                                             const float &value) const;

template void Shader::setExtraUniform<int>(std::string_view id,
                                           const int &value) const;

template void Shader::setExtraUniform<glm::vec3>(std::string_view id,
                                                 const glm::vec3 &value) const;

template void Shader::setExtraUniform<glm::mat4>(std::string_view id,
                                                 const glm::mat4 &value) const;

template void Shader::setExtraUniform<glm::vec2>(std::string_view id,
                                                 const glm::vec2 &value) const;
} // namespace CraftCpp