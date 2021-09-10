//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_OPENGLBUFFER_HPP
#define CPP_OPENGLBUFFER_HPP

#include <vector>
#include <array>
#include "glad/glad.h"
#include "Vertex.hpp"
#include "Wrapper.hpp"

template<typename VertexType, std::enable_if_t<std::is_base_of_v<NormalVertex, VertexType>, bool> = true>
constexpr int get_n_normal_elements(){
    return decltype(VertexType::normal)::length();
}

template<typename VertexType, std::enable_if_t<!std::is_base_of_v<NormalVertex, VertexType>, bool> = true>
constexpr int get_n_normal_elements(){
    return 0;
}

template<typename VertexType, std::enable_if_t<
        std::is_base_of_v<Uv2DVertex, VertexType> || std::is_base_of_v<Uv3DVertex, VertexType>, bool> = true>
constexpr int get_n_uv_elements(){
    return decltype(VertexType::uv)::length();
}

template<typename VertexType, std::enable_if_t<
        !std::is_base_of_v<Uv2DVertex, VertexType> && !std::is_base_of_v<Uv3DVertex, VertexType>, bool> = true>
constexpr int get_n_uv_elements(){
    return 0;
}

template<typename VertexType, std::enable_if_t<std::is_base_of_v<CubeVertex, VertexType>, bool> = true>
constexpr int get_n_matrix_elements(){
        return decltype(VertexType::transform)::length();
}

template<typename VertexType, std::enable_if_t<!std::is_base_of_v<CubeVertex, VertexType>, bool> = true>
constexpr int get_n_matrix_elements(){
    return 0;
}

template<typename VertexType>
constexpr int get_n_pos_elements(){
    return decltype(VertexType::position)::length();
}

template<typename VertexType>
class OpenglBuffer {
private:
    GLuint id;
    static constexpr GLuint STRIDE = sizeof(VertexType);
    void set_vao_attributes() const;
    void _store_data(int size, const GLfloat *data) const;

    mutable size_t n_indices{};
    static constexpr std::array<int,4> attributes_dimensions{
        get_n_pos_elements<VertexType>(),
        get_n_uv_elements<VertexType>(),
        get_n_normal_elements<VertexType>(),
        get_n_matrix_elements<VertexType>()
    };
public:
    OpenglBuffer();
    ~OpenglBuffer();

    void store_data(const std::vector<VertexType>& buffer) const;

    template<size_t n_values>
    void store_data(const std::array<VertexType, n_values>& buffer) const;

    void draw_triangles() const;
    void draw_lines() const;
};

#endif //CPP_OPENGLBUFFER_HPP
