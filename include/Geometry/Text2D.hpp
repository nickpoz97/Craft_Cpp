//
// Created by ultimatenick on 13/08/21.
//

#ifndef CPP_TEXT2D_HPP
#define CPP_TEXT2D_HPP

#include <list>
#include <string_view>

#include "Geometry/Character.hpp"
#include "glm/vec2.hpp"

/**
 * @brief Class used for managing UI texts
 */
namespace CraftCpp {
class Text2D : public RenderableEntity<Uv2DVertex> {
private:
  using SuperClass = RenderableEntity<Uv2DVertex>;

  static std::vector<Uv2DVertex> genBuffer(const glm::vec2 &position, float n,
                                           std::string_view text);

public:
  /// @brief center position of initial Character
  glm::ivec2 initialCharPos;

  /**
   * @brief instantiates a 2D text of a specified size in a specified position
   * @param position position in pixels of the first Character
   * @param n size in pixels of each Character
   * @param text string to be rendered
   */
  Text2D(const glm::vec2 &position, float n, std::string_view text);

  /**
   * @brief renders text on screen
   * @note this method overrides a non virtual method
   */
  void renderObject() const;
};
} // namespace CraftCpp

#endif // CPP_TEXT2D_HPP
