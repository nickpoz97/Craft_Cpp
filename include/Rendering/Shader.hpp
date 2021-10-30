//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_SHADER_HPP
#define CPP_SHADER_HPP

#include <string_view>
#include <array>

#include "glad/include/glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace CraftCpp {
struct Shader {
private:
    enum class shader_type {
        VERTEX, FRAGMENT
    };

    GLuint id;

    static int build_shader(std::string_view path, shader_type st);

    static void _set_extra_uniform(int u_location, int val);

    static void _set_extra_uniform(int u_location, float val);

    static void _set_extra_uniform(int u_location, const glm::vec3 &val);

    static void _set_extra_uniform(int u_location, const glm::vec2 &val);

    static void _set_extra_uniform(int u_location, const glm::mat4 &val);

    static void check_compile_errors(GLuint shader, std::string type);

    void set_pi() const;

public:
    void use() const;

    Shader(std::string_view vs_path, std::string_view fs_path);

    GLuint get_id() const;

    void set_camera(const glm::vec3 &camera_pos) const;

    void set_sampler(int sampler) const;

    void set_timer(float timer) const;

    void set_viewproj(const glm::mat4 &m) const;

    template<typename GLtype>
    void set_extra_uniform(std::string_view uniform_name, const GLtype &value) const;
};
}

#endif //CPP_SHADER_HPP
