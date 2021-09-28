//
// Created by ultimatenick on 28/09/21.
//

#include "catch.hpp"
#include "Geometry/BlockMap.hpp"
#include "Geometry/Chunk.hpp"
#include "Geometry/TileBlock.hpp"

TEST_CASE("BlockMap initialization and insertion", "[initialization][data_insertion]"){
    auto pq = GENERATE(glm::ivec2{6,9}, glm::ivec2{0,0}, glm::ivec2{-3,-8}, glm::vec2{-3.2, 6.9});
    auto block_type = GENERATE(BlockType::GRASS, BlockType::SAND, BlockType::EMPTY);
    BlockMap b{pq};
    REQUIRE(b.get_pq() == static_cast<glm::ivec2>(pq));
    REQUIRE(b.get_delta() == glm::ivec3(pq.x, 0, pq.y) * Chunk::SIZE - glm::ivec3{1, 0, 1});

    auto position = GENERATE(glm::ivec3{-4, 6, 7}, glm::ivec3{0,0,0}, glm::vec3{5.9, 4.2, 7.9});
    b.set_block(position, block_type);
    REQUIRE(b.at(position).get_index() == block_type);

    REQUIRE(b.at({1,1,1}).is_empty());
}