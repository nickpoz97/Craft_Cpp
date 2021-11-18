//
// Created by ultimatenick on 09/08/21.
//

#ifndef CPP_MAP_UTILS_HPP
#define CPP_MAP_UTILS_HPP

#include <unordered_map>
#include "glm/vec3.hpp"
#include "Geometry/TileBlock.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

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
}

/**
 * @brief hash for unordered map which use ivec3 as key
 */
template<>
struct std::hash<glm::ivec3> {
    std::size_t operator()(const glm::ivec3 &v) const;
};

/**
 * @brief hash for unordered map which use ivec2 as key
 */
template<>
struct std::hash<glm::ivec2> {
    std::size_t operator()(const glm::ivec2 &v) const;
};
#endif //CPP_MAP_UTILS_HPP
