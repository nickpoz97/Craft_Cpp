/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPP_CUBICOBJECT_HPP
#define CPP_CUBICOBJECT_HPP

#include <array>
#include <unordered_map>
#include <vector>

#include "Geometry/RenderableEntity.hpp"
#include "Geometry/Vertex.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "tunableParameters.hpp"

namespace CraftCpp {
/** @brief number of vertices on each BlockObject face
 * @note indices are the vertices of a face (they are called indices since a
 * Block has max 8 vertices, but there are 36 indices since each face is treated
 * independently)
 */
static constexpr int INDICES_FACE_COUNT = 6;

/// @brief iterator of BlockVertex containers
using BlockVertexIterator = typename std::vector<BlockVertex>::iterator;

/**
 * @brief Class that builds Chunk 's blocks
 * @tparam nFaces number of faces of this block
 * @note every block should have 6 faces, but plant does not have the upper and
 * lower one
 * @warning this class is used to fill an external array of block vertices and
 * it only contains iterators to that vertices (no arrays in BlockObject
 * objects) for optimization
 */
template <unsigned nFaces> class BlockObject {
private:
  BlockVertexIterator beginIterator;
  size_t nVertices;
  static constexpr float N = 0.5f;

  // matrix of BlockObject 's local positions
  //  coordinate [i][j] indexes i-th face and j-th vertex-position of that face
  using PositionsMatrix = std::array<std::array<glm::vec3, 4>, nFaces>;

  // matrix of BlockObject 's local positions
  // coordinate [i][j] indexes i-th face and j-th uv flag and position of that
  // face
  using IndicesMatrix = std::array<std::array<int, INDICES_FACE_COUNT>, nFaces>;

  // array of BlockObject' s normals
  // every element is the normal vector of a face
  using NormalMatrix = std::array<glm::vec3, nFaces>;

  // matrix of BlockObject' s uv flags
  // these flags are used to choosing between A and B coordinate offsets
  using UvsMatrix = std::array<std::array<glm::bvec2, 4>, nFaces>;

  static constexpr float S = 0.0625;
  static const float A;
  static const float B;

  static const PositionsMatrix localVertexPositions;
  static const UvsMatrix uvs;
  static const IndicesMatrix indices;
  static const NormalMatrix normals;

public:
  /// @brief total number of indices for a BlockType object
  static constexpr int maxIndices = INDICES_FACE_COUNT * nFaces;

  /**
   * @brief Block with a specified type, position and rotation
   * @param[in] blockType type of block (texture depends on this)
   * @param[in] visibleFaces array  of flags for left, right, top, bottom, front,
   * back (true if face is visible)
   * @param[in] centerPosition position of the point in the middle of the Block
   * @param[in] asyRotation rotation around the vertical axis that passes through
   * the centre
   * @param[in] lightObstacles nearby opaque blocks (see Chunk::getLightObstacles)
   * @param[in] verticesIt starting position of container to be filled with the
   * vertices of this block
   * @warning every BlockObject occupies INDICES_FACE_COUNT * nFaces vertices
   */
  BlockObject(const BlockType &blockType,
              const std::array<bool, 6> &visibleFaces,
              const glm::vec3 &centerPosition, float asyRotation,
              const std::unordered_map<glm::ivec3, bool> &lightObstacles,
              BlockVertexIterator verticesIt);

  /// @return iterator position after end of BlockObject vertices
  [[nodiscard]] BlockVertexIterator end() const;
  /// @return iterator position of the first BlockObject vertex
  [[nodiscard]] BlockVertexIterator begin() const;
};

/// @brief BlockObject with only lateral faces
class Plant : public BlockObject<4> {
private:
  using super = BlockObject<4>;

public:
  /// @brief see BlockObject() for parameters
  Plant(const BlockType &blockType, const glm::vec3 &centerPosition,
        float asy_rotation, BlockVertexIterator verticesIt,
        const std::unordered_map<glm::ivec3, bool> &lightObstacles);
};

/// @brief BlockObject with no rotation (Cube objects can be tiled to form a
/// Chunk)
class Cube : public BlockObject<6> {
private:
  using super = BlockObject<6>;

public:
  /// @brief see BlockObject() for parameters
  Cube(const BlockType &blockType, const std::array<bool, 6> &visibleFaces,
       const glm::vec3 &position, BlockVertexIterator verticesIt,
       const std::unordered_map<glm::ivec3, bool> &lightObstacles);
};
} // namespace CraftCpp
#endif // CPP_CUBICOBJECT_HPP
