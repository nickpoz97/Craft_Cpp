/**
 * @copyright CppCraft
 * @author Niccolò Pozzetti
 * @version v1.0
 * @date November, 2021
 * @file
 */

#ifndef CPP_CHUNK_HPP
#define CPP_CHUNK_HPP

#include <array>
#include <list>
#include <thread>
#include <unordered_map>

#include "Geometry/RenderableEntity.hpp"
#include "Geometry/Vertex.hpp"
#include "Geometry/mapUtils.hpp"
#include "glm/vec2.hpp"
#include "tunableParameters.hpp"

namespace CraftCpp {

/** @brief tiling of BlockObject objects built with Perlin noise
 * @note this is an atomic entity (every BlockObject is aggregated into a unique
 * BlockVertex buffer)
 * @note Chunk can be viewed as a box with Chunk::SIZE as length and width and
 * CHUNK::Y_LIMIT as height
 * @note total blocks (including the empty ones): Chunk::SIZE * Chunk::SIZE *
 * Chunk::Y_LIMIT
 */
class Chunk;

/// @brief map where key is Chunk 's pq coordinate and value should be the Chunk
/// in that position
using ChunkMap = std::unordered_map<glm::ivec2, Chunk>;

class Chunk : public RenderableEntity<BlockVertex> {
private:
  mutable std::thread initChunkThread{};
  using SuperClass = RenderableEntity<BlockVertex>;
  using BufferType = std::vector<BlockVertex>;

  using BlockMap = std::unordered_map<glm::ivec3, BlockType>;
  mutable BufferType localBuffer;
  BlockMap blockMap{};

  mutable bool renderReady{false};
  mutable bool localBufferReady{false};

  void updateBuffer() const;
  int countExposedFaces() const;

  BufferType::iterator
  generateBlockGeometry(BufferType::iterator vertexIt,
                        const glm::ivec3 &blockPos, TileBlock tileBlock,
                        const std::array<bool, 6> &visibleFaces) const;

  void computeChunkGeometry() const;

  std::array<glm::ivec3, 8> getChunkBoundaries() const;

  const std::array<glm::ivec2, 4> xzBoundaries;

  bool checkBorder(const glm::ivec3 &pos, const ::glm::ivec3 &direction) const;

  std::array<bool, 6> getVisibleFaces(TileBlock w, const glm::ivec3 &pos) const;

  static int chunked(int val);

  static glm::ivec2 getMinXz(const glm::ivec2 &pq);
  static glm::ivec2 getMaxXz(const glm::ivec2 &pq);
  static std::array<glm::ivec2, 4> computeXZBoundaries(const glm::ivec2 &pq);

  void generateBlockmap();

public:
  /**
   * @brief builds a Chunk object at specified coordinated
   * @param[in] pqCoordinates Chunk coordinates
   * @note Chunks are tiled using this coordinate system
   * @note xz corners are computed with Chunk::computeXZBoundaries (these are in
   * block-coordinate system)
   */
  explicit Chunk(const glm::ivec2 &pqCoordinates);

  static constexpr int SIZE = CHUNK_SIZE; ///< number of blocks per chunk side
  static constexpr int Y_LIMIT = 256; ///< max vertical blocks per chunk object
  const glm::ivec2 pq{};              ///< Chunk coordinate

  /**
   * @defgroup ChunkCornersGetters Chunk corners getters
   * @{
   */
  int getMinX() const;
  int getMinZ() const;
  int getMaxX() const;
  int getMaxZ() const;
  /// @}

  /// @returns y-block-coordinate of highest non-cloud and non-empty block
  int getHighestBlock() const;

  /**
   * @brief get type of block at given absolute pos
   * @param[in] blockPos block-coordinate of the block
   * @returns TileBlock::BlockType of block at given block-coordinate
   * @note empty block is returned if no block is present in that position
   */
  BlockType getBlock(const glm::ivec3 &blockPos) const;
  /** @brief set block type of block specified at block-coordinate
   * @param[in] position block-coordinate of block to set
   * @param[in] w block type
   */
  void setBlock(const glm::ivec3 &position, BlockType w);
  /**
   * @brief check if this Chunk is visible from the camera
   * @param[in] viewproj matrix projection*view matrix
   * @return true iff Chunk is visible from camera
   */
  bool isVisible(const glm::mat4 &viewproj) const;

  /**
   * @brief check if local geometry buffer (that will be copied to GPU) is ready
   * @return true iff local geometry buffer has been computed
   */
  bool isLocalBufferReady() const;

  // thread not running and not initialized
  /**
   * @brief check if Chunk initialization is running or has never been called
   * @return false iff Chunk has been initialized
   */
  bool needInitCall() const;
  /**
   * @brief transforms 3d coordinate into chunk-coordinate
   * @param[in] position block coordinate
   * @return chunk-coordinate of input parameter
   */
  static glm::ivec2 chunked(const glm::vec3 &position);

  /**
   * @brief renders object if Chunk is initialized and GPU buffer is ready
   * @note it overrides RenderableEntity::renderObject (which is not virtual)
   */
  void renderObject() const;

  /**
   * @brief calls a thread that sets Chunk' s blocks using Perlin noise
   */
  void initChunk();

  // first is top-left, going counterclockwise
  /**
   * @brief get light obstacles for a block-coordinate (used for Ambient
   * Occlusion)
   * @param[in] blockPos block-coordinate to test
   * @return map where key is the block-coordinate offset wrt input
   * block-coordinate and value is a flag that is true iff there is a opaque
   * block in blockPos + offset position
   */
  std::unordered_map<glm::ivec3, bool>
  getLightObstacles(const glm::ivec3 &blockPos) const;

  /**
   * @brief stops execution until initialization thread has finished
   * @note does nothing if Chunk has never been initialized
   */
  void waitThread() const;

  /**
   * @brief check if specified block-coordinate is on Chunk object border or
   * outside
   * @param[in] pos block-coordinate to be tested
   * @return true iff specified block-coordinate is on Chunk object border or
   * outside
   */
  bool isOnBorder(const glm::ivec3 &pos) const;
};

/**
 * @brief get the distance of 2 chunk-coordinates
 * @param[in] pq1 first chunk-coordinate
 * @param[in] pq2 second chunk-coordinate
 * @return the highest of the 2 coefficients of pq1 - pq2
 */
int getChunkDistance(const glm::ivec2 &pq1, const glm::ivec2 &pq2);

} // namespace CraftCpp

#endif // CPP_CHUNK_HPP
