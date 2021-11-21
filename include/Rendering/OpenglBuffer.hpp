//
// Created by ultimatenick on 11/08/21.
//

#ifndef CPP_OPENGLBUFFER_HPP
#define CPP_OPENGLBUFFER_HPP

#include <array>
#include <vector>

#include "Geometry/Vertex.hpp"
#include "Rendering/OpenGlExternal.hpp"
#include "Rendering/attributesSize.hpp"

namespace CraftCpp {

/**
 * @brief handler for Opengl VBO and VAO buffers
 * @tparam VertexType struct in Vertex.hpp used to set VAO
 */
template <typename VertexType> class OpenglBuffer {
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
      getPosAttributeSize<VertexType>(), getUvAttributeSize<VertexType>(),
      getNormalAttributeSize<VertexType>(), getAoAttributeSize<VertexType>()};

public:
  /**
   * @brief allocates new VAO and VBO buffers in the gpu if GameView have been
   * instantiated
   * @note if you are not testing it, don' t use this without setting GameView
   * instance
   */
  explicit OpenglBuffer();

  explicit OpenglBuffer(const OpenglBuffer<VertexType> &other) = delete;

  /// @brief takes ownership of another gpu buffer
  OpenglBuffer(OpenglBuffer<VertexType> &&other) noexcept;

  OpenglBuffer<VertexType> &
  operator=(const OpenglBuffer<VertexType> &other) = delete;

  /// @brief takes ownership of another gpu buffer
  OpenglBuffer<VertexType> &
  operator=(OpenglBuffer<VertexType> &&other) noexcept;

  /// releases memory occupied by buffers in gpu
  ~OpenglBuffer();

  /**
   * @brief copies a buffer into gpu VBO
   * @param[in] buffer buffer to be copied
   */
  void storeData(const std::vector<VertexType> &buffer) const;

  /// @brief writes on display buffer using triangle mode
  void drawTriangles() const;
  /// @brief writes on display buffer using line mode
  void drawLines() const;
};
} // namespace CraftCpp
#endif // CPP_OPENGLBUFFER_HPP
