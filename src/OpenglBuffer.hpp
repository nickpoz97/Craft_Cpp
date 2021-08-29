//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_OPENGLBUFFER_HPP
#define CPP_OPENGLBUFFER_HPP

#include <vector>
#include "glad/glad.h"
#include "AttributesWrapper.hpp"
#include "Vertex.hpp"

template<typename VertexType, std::enable_if_t<std::is_base_of_v<NormalVertex, VertexType>, bool> = true>
struct N_NORMAL_ELEMENTS{
    static constexpr GLuint v = decltype(VertexType::normal)::length();
};

template<typename VertexType, std::enable_if_t<std::is_base_of_v<Uv2DVertex, VertexType> ||
        std::is_base_of_v<Uv3DVertex, VertexType>, bool> = true>
struct N_UV_ELEMENTS{
    static constexpr GLuint v = decltype(VertexType::uv)::length();
};

template<typename VertexType, std::enable_if_t<std::is_base_of_v<CubeVertex, VertexType>, bool> = true>
struct N_AOLIGHT_ELEMENTS{
    static constexpr GLuint v = decltype(VertexType::ao_light)::length();
};

template<typename VertexType, std::enable_if_t<std::is_base_of_v<CubeVertex, VertexType>, bool> = true>
struct N_MATRIX_ELEMENTS{
    static constexpr GLuint v = decltype(VertexType::transform)::length();
};

template<typename VertexType>
struct N_POS_ELEMENTS{
    static constexpr GLuint v = decltype(VertexType::position)::length();
};

template<typename VertexType>
class OpenglBuffer {
private:
    GLuint id;
    static constexpr GLuint STRIDE = sizeof(VertexType);
    const AttributesWrapper& attrib;
    void set_vao_attributes() const;
public:
    OpenglBuffer();
    ~OpenglBuffer();

    void store_data(GLsizei size, const GLfloat* const data) const;
    void store_data(const std::vector<VertexType>& buffer);
    void draw_triangles(int count) const;
    void draw_lines(int components, int count) const ;
};

#endif //CPP_OPENGLBUFFER_HPP
