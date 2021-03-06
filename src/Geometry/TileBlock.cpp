//
// Created by ultimatenick on 07/08/21.
//

#include "Geometry/TileBlock.hpp"

namespace CraftCpp {
decltype(TileBlock::tiles) TileBlock::tiles{{
    {0, 0, 0, 0, 0, 0},             // 0 - empty
    {16, 16, 32, 0, 16, 16},        // 1 - grass
    {1, 1, 1, 1, 1, 1},             // 2 - sand
    {2, 2, 2, 2, 2, 2},             // 3 - stone
    {3, 3, 3, 3, 3, 3},             // 4 - brick
    {20, 20, 36, 4, 20, 20},        // 5 - wood
    {5, 5, 5, 5, 5, 5},             // 6 - cement
    {6, 6, 6, 6, 6, 6},             // 7 - dirt
    {7, 7, 7, 7, 7, 7},             // 8 - plank
    {24, 24, 40, 8, 24, 24},        // 9 - snow
    {9, 9, 9, 9, 9, 9},             // 10 - glass
    {10, 10, 10, 10, 10, 10},       // 11 - cobble
    {11, 11, 11, 11, 11, 11},       // 12 - light stone
    {12, 12, 12, 12, 12, 12},       // 13 - dark stone
    {13, 13, 13, 13, 13, 13},       // 14 - chest
    {14, 14, 14, 14, 14, 14},       // 15 - leaves
    {15, 15, 15, 15, 15, 15},       // 16 - cloud
    {48, 48, 0, 0, 48, 48},         // 17 - tall grass
    {49, 49, 0, 0, 49, 49},         // 18 - yellow flower
    {50, 50, 0, 0, 50, 50},         // 19 - red flower
    {51, 51, 0, 0, 51, 51},         // 20 - purple flower
    {52, 52, 0, 0, 52, 52},         // 21 - sun flower
    {53, 53, 0, 0, 53, 53},         // 22 - white flower
    {54, 54, 0, 0, 54, 54},         // 23 - blue flower
    {0, 0, 0, 0, 0, 0},             // 24
    {0, 0, 0, 0, 0, 0},             // 25
    {0, 0, 0, 0, 0, 0},             // 26
    {0, 0, 0, 0, 0, 0},             // 27
    {0, 0, 0, 0, 0, 0},             // 28
    {0, 0, 0, 0, 0, 0},             // 29
    {0, 0, 0, 0, 0, 0},             // 30
    {0, 0, 0, 0, 0, 0},             // 31
    {176, 176, 176, 176, 176, 176}, // 32
    {177, 177, 177, 177, 177, 177}, // 33
    {178, 178, 178, 178, 178, 178}, // 34
    {179, 179, 179, 179, 179, 179}, // 35
    {180, 180, 180, 180, 180, 180}, // 36
    {181, 181, 181, 181, 181, 181}, // 37
    {182, 182, 182, 182, 182, 182}, // 38
    {183, 183, 183, 183, 183, 183}, // 39
    {184, 184, 184, 184, 184, 184}, // 40
    {185, 185, 185, 185, 185, 185}, // 41
    {186, 186, 186, 186, 186, 186}, // 42
    {187, 187, 187, 187, 187, 187}, // 43
    {188, 188, 188, 188, 188, 188}, // 44
    {189, 189, 189, 189, 189, 189}, // 45
    {190, 190, 190, 190, 190, 190}, // 46
    {191, 191, 191, 191, 191, 191}, // 47
    {192, 192, 192, 192, 192, 192}, // 48
    {193, 193, 193, 193, 193, 193}, // 49
    {194, 194, 194, 194, 194, 194}, // 50
    {195, 195, 195, 195, 195, 195}, // 51
    {196, 196, 196, 196, 196, 196}, // 52
    {197, 197, 197, 197, 197, 197}, // 53
    {198, 198, 198, 198, 198, 198}, // 54
    {199, 199, 199, 199, 199, 199}, // 55
    {200, 200, 200, 200, 200, 200}, // 56
    {201, 201, 201, 201, 201, 201}, // 57
    {202, 202, 202, 202, 202, 202}, // 58
    {203, 203, 203, 203, 203, 203}, // 59
    {204, 204, 204, 204, 204, 204}, // 60
    {205, 205, 205, 205, 205, 205}, // 61
    {206, 206, 206, 206, 206, 206}, // 62
    {207, 207, 207, 207, 207, 207}, // 63
}};

bool TileBlock::isPlant() const {
  switch (tileCubeIndex) {
  case BlockType::TALL_GRASS:
  case BlockType::YELLOW_FLOWER:
  case BlockType::RED_FLOWER:
  case BlockType::PURPLE_FLOWER:
  case BlockType::SUN_FLOWER:
  case BlockType::WHITE_FLOWER:
  case BlockType::BLUE_FLOWER:
    return true;
  default:
    return false;
  }
}

bool TileBlock::isObstacle() const {
  if (isPlant()) {
    return false;
  }
  switch (tileCubeIndex) {
  case BlockType::EMPTY:
  case BlockType::CLOUD:
    return false;
  default:
    return true;
  }
}

bool TileBlock::isTransparent() const {
  if (isPlant()) {
    return true;
  }
  switch (tileCubeIndex) {
  case BlockType::EMPTY:
  case BlockType::GLASS:
  case BlockType::LEAVES:
    return true;
  default:
    return false;
  }
}

bool TileBlock::isEmpty() const { return tileCubeIndex == BlockType::EMPTY; }

TileBlock::operator BlockType() const { return tileCubeIndex; }

BlockType TileBlock::getBlockType() const { return tileCubeIndex; }

TileBlock::TileBlock(BlockType blockType) : tileCubeIndex{blockType} {}

TileCube::Iterator TileBlock::begin() const {
  return tiles[getBlockType()].begin();
}

TileCube::Iterator TileBlock::end() const {
  return tiles[getBlockType()].end();
}

bool TileBlock::isPartOfTerrain() {
  return !(tileCubeIndex == BlockType::CLOUD ||
           tileCubeIndex == BlockType::EMPTY);
}

TileCube::Iterator TileCube::begin() const {
  return {reinterpret_cast<const int *>(this)};
}

TileCube::Iterator TileCube::end() const {
  const int *begin_ref{reinterpret_cast<const int *>(this)};
  return {begin_ref + 6};
}

TileCube::Iterator::Iterator(const int *tile_pointer)
    : tile_pointer{tile_pointer} {}

int TileCube::Iterator::operator*() { return *tile_pointer; }

const int *TileCube::Iterator::operator->() { return tile_pointer; }

TileCube::Iterator &TileCube::Iterator::operator++() {
  ++tile_pointer;
  return *this;
}

bool TileCube::Iterator::operator==(const TileCube::Iterator &b) {
  return tile_pointer == b.tile_pointer;
}

bool TileCube::Iterator::operator!=(const TileCube::Iterator &b) {
  return tile_pointer != b.tile_pointer;
}
} // namespace CraftCpp