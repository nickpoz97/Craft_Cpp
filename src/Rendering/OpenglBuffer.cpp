//
// Created by ultimatenick on 11/08/21.
//

#include "glad/include/glad/glad.h"

#include "glm/vec3.hpp"
#include "Rendering/OpenglBuffer.hpp"

namespace CraftCpp {
template<typename VertexType>
OpenglBuffer<VertexType>::OpenglBuffer(bool openGLReady) {
    if (openGLReady) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
}

template<typename VertexType>
OpenglBuffer<VertexType>::~OpenglBuffer() {
    if (VAO && VBO) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

template<typename VertexType>
void OpenglBuffer<VertexType>::drawTriangles() const {
    if(!VBO || !VAO){
        return;
    }

    bindBuffer();

    setVaoAttributes();
    for (int i = 0, j = 0; i < attributesDimensions.size(); ++i) {
        if (attributesDimensions[i] != 0) {
            glEnableVertexAttribArray(j++);
        };
    }
    glDrawArrays(GL_TRIANGLES, 0, nIndices);
    for (int i = 0, j = 0; i < attributesDimensions.size(); ++i) {
        if (attributesDimensions[i] != 0) { glDisableVertexAttribArray(j++); };
    }
    unbindBuffer();
}

template<typename VertexType>
void OpenglBuffer<VertexType>::setVaoAttributes() const {
    size_t offset = 0;
    int i = 0;

    for (int a_dim: attributesDimensions) {
        if (a_dim != 0) {
            glVertexAttribPointer(i++, a_dim, GL_FLOAT, GL_FALSE, STRIDE, (GLvoid *) offset);
            offset += a_dim * sizeof(float);
        }
    }
}

template<typename VertexType>
void OpenglBuffer<VertexType>::drawLines() const {
    if(!VBO || !VAO){
        return;
    }

    bindBuffer();
    glVertexAttribPointer(0, getPosAttributeSize<VertexType>(), GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINES, 0, nIndices);
    glDisableVertexAttribArray(0);
    unbindBuffer();
}

template<typename VertexType>
void OpenglBuffer<VertexType>::storeData(const std::vector<VertexType> &buffer) const {
    if(!VBO || !VAO){
        return;
    }

    //_store_data(sizeof(buffer), reinterpret_cast<const GLfloat*>(buffer.data()));
    _storeData(buffer.size() * sizeof(VertexType), reinterpret_cast<const GLfloat *>(buffer.data()));

    nIndices = buffer.size();
}

template<typename VertexType>
void OpenglBuffer<VertexType>::_storeData(int size, const GLfloat *data) const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
}

template<typename VertexType>
void OpenglBuffer<VertexType>::bindBuffer() const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
}

template<typename VertexType>
void OpenglBuffer<VertexType>::unbindBuffer() const {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename VertexType>
OpenglBuffer<VertexType>::OpenglBuffer(OpenglBuffer<VertexType> &&other) noexcept :
        VAO{other.VAO}, VBO{other.VBO}, nIndices{other.nIndices} {

    other.VAO = 0;
    other.VBO = 0;
}

template<typename VertexType>
OpenglBuffer<VertexType> &OpenglBuffer<VertexType>::operator=(OpenglBuffer<VertexType> &&other) noexcept {
    VAO = other.VAO;
    VBO = other.VBO;
    other.VAO = 0;
    other.VBO = 0;

    return *this;
}

template
class OpenglBuffer<Standard2DVertex>;

template
class OpenglBuffer<Uv2DVertex>;

template
class OpenglBuffer<Standard3DVertex>;

template
class OpenglBuffer<NormalVertex>;

template
class OpenglBuffer<Uv3DVertex>;

template
class OpenglBuffer<CubeVertex>;
}