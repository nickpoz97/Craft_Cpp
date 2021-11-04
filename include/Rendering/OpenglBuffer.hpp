//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_OPENGLBUFFER_HPP
#define CPP_OPENGLBUFFER_HPP

#include <vector>
#include <array>

#include "Rendering/OpenGlExternal.hpp"
#include "Geometry/Vertex.hpp"
#include "Rendering/attributesSize.hpp"

namespace CraftCpp {

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
        getPosAttributeSize<VertexType>(),
        getUvAttributeSize<VertexType>(),
        getNormalAttributeSize<VertexType>(),
        getAoAttributeSize<VertexType>()
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
