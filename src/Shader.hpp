//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_SHADER_HPP
#define CPP_SHADER_HPP


#include <string_view>
#include "glad/glad.h"
#include "UniformsWrapper.hpp"

struct Shader {
private:
    GLuint id;
    int build_shader(std::string_view code_path);

public:
    const UniformWrapper& uniforms;
    void use();

    Shader(std::string_view vs_path, std::string_view fs_path, const UniformWrapper& uniforms);
    GLuint get_id() const;
};


#endif //CPP_SHADER_HPP
