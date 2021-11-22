/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPP_MAP_UTILS_HPP
#define CPP_MAP_UTILS_HPP

#include "Geometry/TileBlock.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <unordered_map>

namespace CraftCpp {
/**
 * @brief hash coefficient optimized for aggregations of int values
 * @param[in] val hash is computed on this value
 * @return hash value
 */
std::size_t hashCoefficient(int val);

/// @brief class for y-wise vector comparison
struct yCoordComparator {
  /// @brief compares 2 3d vectors by y-coordinate
  /// @return true iff a.y < b.y
  bool operator()(const glm::vec3 &a, const glm::vec3 &b) const;
};
} // namespace CraftCpp

namespace std {
/**
 * @brief hash for unordered map which use ivec3 as key
 */
template <> struct hash<glm::ivec3> {
  size_t operator()(const glm::ivec3 &v) const;
};

/**
 * @brief hash for unordered map which use ivec2 as key
 */
template <> struct hash<glm::ivec2> {
  size_t operator()(const glm::ivec2 &v) const;
};
} // namespace std
#endif // CPP_MAP_UTILS_HPP
