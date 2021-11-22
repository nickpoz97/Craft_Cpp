/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPP_SPHERE_HPP
#define CPP_SPHERE_HPP

#include <array>
#include <vector>

#include "Geometry/RenderableEntity.hpp"
#include "Geometry/Vertex.hpp"
#include "Rendering/OpenglBuffer.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

/**
 * @brief 3D skybox
 * @warning this class has never been tested
 */
namespace CraftCpp {
class Sphere : public RenderableEntity<Uv3DVertex> {
private:
  using SuperClass = RenderableEntity<Uv3DVertex>;
  static constexpr int N_TRIANGLES = 8;

  static std::array<glm::vec3, N_TRIANGLES> triangles_indices;
  static std::array<glm::vec3, 6> positions;
  static std::array<glm::vec2, 6> uvs;

  const float r;

  using iterator_type = std::vector<Uv3DVertex>::iterator;

  iterator_type rec_gen_sphere(
      int detail, const std::array<glm::vec3, 3> &actual_pos_triangle,
      const std::array<glm::vec2, 3> &actual_uvs_triangle, iterator_type it);

  iterator_type
  fill_local_buffer(const std::array<glm::vec3, 3> &actual_pos_triangle,
                    const std::array<glm::vec2, 3> &actual_uvs_triangle,
                    iterator_type it) const;

public:
  /**
   * @brief creates a Sphere of a specified radius and detail
   * @param[in] r radius of the sphere
   * @param[in] detail number of triangles
   */
  Sphere(float r, int detail);
};
} // namespace CraftCpp

#endif // CPP_SPHERE_HPP
