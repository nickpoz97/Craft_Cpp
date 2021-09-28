//
// Created by ultimatenick on 28/09/21.
//

#include "Geometry/Chunk.hpp"
#include "catch.hpp"

TEST_CASE("Chunk instantiation and insertion", "[instantiation]"){
    auto pq_coords = GENERATE(glm::vec2{4.7,6.9}, glm::vec2{9,10});
    auto obstacle_block_pos = GENERATE(glm::vec3{5.1, 4.3, 9.6}, glm::vec3{6, 8, 9});
    Chunk c{pq_coords, false};
    REQUIRE(!c.is_dirty());

    REQUIRE(c.pq == static_cast<glm::ivec2>(pq_coords));
    c.set_block(obstacle_block_pos, BlockType::SAND);
    REQUIRE(c.is_dirty());

    REQUIRE(c.get_block(obstacle_block_pos).get_index() == BlockType::SAND);
    REQUIRE(c.getHighestBlock() == static_cast<int>(obstacle_block_pos.y));
    // no block here
    REQUIRE(c.get_block(obstacle_block_pos + glm::vec3{1, 0, 0}).is_empty());

    auto void_block_pos = static_cast<glm::ivec3>(obstacle_block_pos) + glm::ivec3{2.0, 5.0, 1.0};
    c.set_block(void_block_pos, BlockType::CLOUD);
    REQUIRE(c.is_dirty());
    REQUIRE(c.get_block(void_block_pos).get_index() == BlockType::CLOUD);
    REQUIRE(c.getHighestBlock() == static_cast<int>(obstacle_block_pos.y));
}
