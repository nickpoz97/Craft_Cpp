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
    const UniformWrapper& uniforms{};
    int build_shader(std::string_view code_path);
    void get_uniforms_location();

    template<typename GLtype>
    void _set_extra<GLtype>(int u_location, GLtype val) const;
public:
    void use() const;

    Shader(std::string_view vs_path, std::string_view fs_path, const UniformsWrapper &uniforms = {});
    GLuint get_id() const;

    void set_camera(const glm::vec3& camera_pos) const;
    void set_sampler(int sampler) const;
    void set_timer(float timer) const;
    void set_viewproj(const glm::mat4& m) const;

    template<typename GLtype>
    void set_extra<GLtype>(int extra_suffix, GLtype value) const;
};


#endif //CPP_SHADER_HPP
