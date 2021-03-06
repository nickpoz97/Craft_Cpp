/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPP_CROSSHAIR_HPP
#define CPP_CROSSHAIR_HPP

#include "Geometry/RenderableEntity.hpp"
#include "Geometry/Vertex.hpp"
#include "Rendering/Shader.hpp"

namespace CraftCpp {

/// @brief Class that manages gpu buffer and rendering of crosshair UI
class Crosshair : public RenderableEntity<Standard2DVertex> {
private:
  using SuperClass = RenderableEntity<Standard2DVertex>;
  static std::vector<Standard2DVertex> getEndsCoordinates(int width, int height,
                                                          int scale);

public:
  /// @brief 2D pixel coordinates of Crosshair center position
  glm::ivec2 centerPos;

  /**
   * @brief Constructs a renderable Crosshair in the centre of the window
   * @param[in] width width of the screen
   * @param[in] height height of the screen
   * @param[in] scale size of the crosshair (vertical and horizontal bar)
   */
  Crosshair(int width, int height, int scale);

  void renderObject() = delete;

  /**
   * @brief renders crosshair
   * @note color depends on background
   * @note overrides superclass non virtual method
   */
  void renderLines() const;
};

} // namespace CraftCpp

#endif // CPP_CROSSHAIR_HPP
