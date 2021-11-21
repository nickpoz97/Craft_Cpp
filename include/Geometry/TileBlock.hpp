//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_TILEBLOCK_HPP
#define CPP_TILEBLOCK_HPP

#include <array>
#include <vector>

#include "glm/vec3.hpp"

namespace CraftCpp {

/// @brief types of BlockObject
enum BlockType {
  EMPTY,
  GRASS,
  SAND,
  STONE,
  BRICK,
  WOOD,
  CEMENT,
  DIRT,
  PLANK,
  SNOW,
  GLASS,
  COBBLE,
  LIGHT_STONE,
  DARK_STONE,
  CHEST,
  LEAVES,
  CLOUD,
  TALL_GRASS,
  YELLOW_FLOWER,
  RED_FLOWER,
  PURPLE_FLOWER,
  SUN_FLOWER,
  WHITE_FLOWER,
  BLUE_FLOWER,
  COLOR_00,
  COLOR_01,
  COLOR_02,
  COLOR_03,
  COLOR_04,
  COLOR_05,
  COLOR_06,
  COLOR_07,
  COLOR_08,
  COLOR_09,
  COLOR_10,
  COLOR_11,
  COLOR_12,
  COLOR_13,
  COLOR_14,
  COLOR_15,
  COLOR_16,
  COLOR_17,
  COLOR_18,
  COLOR_19,
  COLOR_20,
  COLOR_21,
  COLOR_22,
  COLOR_23,
  COLOR_24,
  COLOR_25,
  COLOR_26,
  COLOR_27,
  COLOR_28,
  COLOR_29,
  COLOR_30,
  COLOR_31
};

/// @brief u and v indices of the first vertex of each face
/// @note this indices will be used by BlockObject() to obtain the right uv
/// coordinate from texture file
struct TileCube {
  /**
   * @defgroup UvIndices BlockObject 's faces UV indices
   * @brief uv index of each BlockObject face
   * @{
   */
  int leftFace;
  int rightFace;
  int topFace;
  int bottomFace;
  int frontFace;
  int backFace;
  /// @}

  /// @brief iterates thrugh faces
  class Iterator : public std::iterator<std::output_iterator_tag, int> {
  public:
    Iterator(const int *tile_pointer);

    int operator*();

    const int *operator->();

    Iterator &operator++();

    bool operator==(const Iterator &b);

    bool operator!=(const Iterator &b);

  private:
    const int *tile_pointer;
  };

  [[nodiscard]] Iterator begin() const;
  [[nodiscard]] Iterator end() const;
};

/// @brief Wrapper for BlockType and TileCube info
class TileBlock {
private:
  BlockType tileCubeIndex{};

public:
  /// @brief arrays of TileCube structs (1 for each BlockType)
  static const std::array<TileCube, 256> tiles;

  /// @brief obtain BlockType of this object
  [[nodiscard]] BlockType getBlockType() const;

  /**
   * @brief instantiates a TileBlock of specified BlockType
   * @param[in] blockType type of block that you want to instantiate
   */
  TileBlock(BlockType blockType);

  /**
   * @brief instantiates TileBlock with BlockType::EMPTY
   */
  TileBlock() = default;

  /**
   * @brief copies another TileBlock (BlockType of this object changes)
   * @param[in] tileBlock the other TileBlock
   * @return reference to this updated object
   */
  TileBlock &operator=(const TileBlock &tileBlock) = default;

  /**
   * @brief instantiates a TileBLock of Blocktype equal to another
   * @param[in] other the other TileBlock
   */
  TileBlock(const TileBlock &other) = default;

  /**
   * @brief check if this BlockType is one of the plants
   * @return true iff BlockType in the range between BlockType::GRASS (included)
   * and BlockType::BLUE_FLOWER (included)
   */
  [[nodiscard]] bool isPlant() const;

  /**
   * @brief check if this BlockType is one of the obstacles
   * @return true iff BlockType is not BlockType::EMPTY or BlockType::CLOUD or
   * is not a plant
   */
  [[nodiscard]] bool isObstacle() const;

  /**
   * @brief Check if this BlockType blocks light (and casts shadows) or not
   * @return true iff it a plant or BlockType::GLASS or BlockType::LEAVES or
   * BlockType::EMPTY
   */
  [[nodiscard]] bool isTransparent() const;

  /**
   * @brief Check if this is a valid BlockType or an empty one
   * @return true iff BlockType::EMPTY
   * @note this is used when you have to set a Block in a Chunk (you delete it
   * instead of inserting an empty block)
   */
  [[nodiscard]] bool isEmpty() const;

  /**
   * @brief Check if this a BlockType belonging to Chunk terrain
   * @return true iff is not empty or cloud
   */
  [[nodiscard]] bool isPartOfTerrain();

  ///@brief conversion operator used to extract BlockType from this TileBlock
  operator BlockType() const;

  /**
   * @defgroup TileBlockIterators TileBlock Iterators
   * @brief used to iterate through faces
   * @{
   */
  TileCube::Iterator begin() const;
  TileCube::Iterator end() const;
  /// @}
};
} // namespace CraftCpp
#endif // CPP_TILEBLOCK_HPP
