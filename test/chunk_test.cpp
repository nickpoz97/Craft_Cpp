//
// Created by ultimatenick on 28/09/21.
//

#include <forward_list>
#include "Geometry/Chunk.hpp"
#include "catch.hpp"

TEST_CASE("Chunk instantiation (not initialized) and block insertion", "[instantiation]"){
    auto pq_coords = GENERATE(glm::vec2{4.7,6.9}, glm::vec2{9,10});
    auto obstacle_block_pos = GENERATE(glm::vec3{5.1, 4.3, 9.6}, glm::vec3{6, 8, 9});
    Chunk c{pq_coords, false};
    REQUIRE(!c.is_dirty());

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
    REQUIRE(glm::ivec2{c.get_min_x(), c.get_min_z()} == glm::ivec2{pq_coords} * Chunk::SIZE - 1);
    REQUIRE(glm::ivec2{c.get_max_x(), c.get_max_z()} == glm::ivec2{pq_coords} * Chunk::SIZE + Chunk::SIZE);
}

TEST_CASE("Chunk instantiation with initialization", "[instantiation][chunk_initialization]"){
    glm::ivec2 pq_coords{2,2};
    std::forward_list<Chunk> chunk_list{};

    for(int p_offset = 0 ; p_offset < 2 ; p_offset++){
        for(int q_offset = 0 ; q_offset < 2 ; q_offset++){
            chunk_list.emplace_front(pq_coords + glm::ivec2{p_offset, q_offset}, true);
        }
    }
    Chunk::wait_threads();
    for(const Chunk& c : chunk_list) {
        REQUIRE(c.is_dirty());
        REQUIRE(c.getHighestBlock() <= Chunk::get_y_limit());
        for (int x = c.get_min_x(); x <= c.get_max_x(); x++) {
            for (int z = c.get_min_z(); z <= c.get_max_z(); z++) {
                if (x == c.get_min_x() || x == c.get_max_x() || z == c.get_min_z() || z == c.get_max_z()) {
                    REQUIRE(c.get_block({x, 0, z}).get_index() == BlockType::EMPTY);
                }
                else {
                    bool empty_found = false;
                    for(int y = 0 ; y < Chunk::get_y_limit() ; y++) {
                        auto tb = c.get_block({x, y, z});
                        empty_found = empty_found || tb.is_empty();
                        bool assertion = (tb.is_user_buildable() && !empty_found) ||
                            (!tb.is_user_buildable() && empty_found);
                        REQUIRE(assertion);
                    }
                }
            }
        }
    }
}