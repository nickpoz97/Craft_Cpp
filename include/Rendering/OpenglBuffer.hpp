//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_OPENGLBUFFER_HPP
#define CPP_OPENGLBUFFER_HPP

#include <vector>
#include <array>

#include "glad/include/glad/glad.h"
#include "Geometry/Vertex.hpp"
#include "Rendering//Wrapper.hpp"

namespace CraftCpp {
template<typename VertexType, std::enable_if_t<std::is_base_of_v<NormalVertex, VertexType>, bool> = true>
constexpr int getNNormal_elements() {
    return decltype(VertexType::normal)::length();
}

template<typename VertexType, std::enable_if_t<!std::is_base_of_v<NormalVertex, VertexType>, bool> = true>
constexpr int getNNormal_elements() {
    return 0;
}

template<typename VertexType, std::enable_if_t<
        std::is_base_of_v<Uv2DVertex, VertexType> || std::is_base_of_v<Uv3DVertex, VertexType>, bool> = true>
constexpr int getNUvElements() {
    return decltype(VertexType::uv)::length();
}

template<typename VertexType, std::enable_if_t<
        !std::is_base_of_v<Uv2DVertex, VertexType> && !std::is_base_of_v<Uv3DVertex, VertexType>, bool> = true>
constexpr int getNUvElements() {
    return 0;
}

template<typename VertexType, std::enable_if_t<std::is_base_of_v<CubeVertex, VertexType>, bool> = true>
constexpr int getNAoElements() {
    return 1;
}

template<typename VertexType, std::enable_if_t<!std::is_base_of_v<CubeVertex, VertexType>, bool> = true>
constexpr int getNAoElements() {
    return 0;
}

template<typename VertexType>
constexpr int getNPosElements() {
    return decltype(VertexType::position)::length();
}

template<typename VertexType>
class OpenglBuffer {
private:
    GLuint VBO{};
    GLuint VAO{};
    static constexpr GLuint STRIDE = sizeof(VertexType);

    void setVaoAttributes() const;

    void _storeData(int size, const GLfloat *data) const;

    void bindBuffer() const;

    void unbindBuffer() const;

    mutable size_t nIndices{};
    static constexpr std::array<int, 4> attributesDimensions{
            getNPosElements<VertexType>(),
            getNUvElements<VertexType>(),
            getNNormal_elements<VertexType>(),
            getNAoElements<VertexType>()
    };
public:
    explicit OpenglBuffer(bool openGLReady);

    explicit OpenglBuffer() = delete;

    explicit OpenglBuffer(const OpenglBuffer<VertexType> &other) = delete;

    OpenglBuffer(OpenglBuffer<VertexType> &&other) noexcept;

    OpenglBuffer<VertexType> &operator=(const OpenglBuffer<VertexType> &other) = delete;

    OpenglBuffer<VertexType> &operator=(OpenglBuffer<VertexType> &&other) noexcept;

    ~OpenglBuffer();

    void storeData(const std::vector<VertexType> &buffer) const;

    void drawTriangles() const;

    void drawLines() const;
};
}
#endif //CPP_OPENGLBUFFER_HPP
