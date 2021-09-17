//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_SHADER_HPP
#define CPP_SHADER_HPP

#include <string_view>
#include <array>

#include "glad/glad.h"
#include "Wrapper.hpp"
#include "vec3.hpp"
#include "mat4x4.hpp"

struct Shader {
private:
    enum class shader_type{VERTEX, FRAGMENT};

    GLuint id;
    UniformsWrapper uniforms;

    static int build_shader(std::string_view path, shader_type st);
    void get_uniforms_location();

    template<typename GLtype>
    void _set_extra_uniform(int u_location, GLtype val) const;

    static void check_compile_errors(GLuint shader, std::string type);
public:
    void use() const;

    Shader(std::string_view vs_path, std::string_view fs_path);
    GLuint get_id() const;

    void set_camera(const glm::vec3& camera_pos) const;
    void set_sampler(int sampler) const;
    void set_timer(float timer) const;
    void set_viewproj(const glm::mat4& m) const;

    template<typename GLtype>
    void set_extra_uniform(std::string_view uniform_name, GLtype value) const;
};


#endif //CPP_SHADER_HPP
