/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPP_CHARACTER_HPP
#define CPP_CHARACTER_HPP

#include <array>

#include "Geometry/RenderableEntity.hpp"
#include "Geometry/Vertex.hpp"

/**
 * Class containing the array of vertices of a 2D Character
 */
namespace CraftCpp {

class Character {
  static constexpr float s = 0.0625;
  static constexpr float a = s;
  static constexpr float b = s * 2;

  static constexpr int nVertices = 6;
  std::array<Uv2DVertex, nVertices> vertices;

public:
  /// @brief center position of Character object
  glm::ivec2 centerPos;

  /**
   * @defgroup CharacterIterator Character Iterator
   * @brief used to iterate through the vertices of Character object
   * @{
   */
  decltype(vertices.begin()) begin();
  decltype(vertices.end()) end();
  /// @}

  /**
   * @brief Generate a printable 2D character
   * @param[in] position pixel coordinates
   * @param[in] m half of vertical size in pixel
   * @param[in] n half of horizontal size in pixels
   * @param[in] c character to be rendered
   */
  Character(const glm::vec2 &position, float m, float n, char c);

  /// @returns number of vertices used to define the 2D Character
  static int getNVertices();
};

} // namespace CraftCpp

#endif // CPP_CHARACTER_HPP
