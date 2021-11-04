//
// Created by ultimatenick on 28/09/21.
//

#include <forward_list>
#include "Geometry/Chunk.hpp"
#include "catch2/catch.hpp"
#include "glm/gtc/matrix_transform.hpp"

TEST_CASE("Chunk instantiation (not initialized) and block insertion"){
    auto pq_coords = GENERATE(glm::vec2{4.7,6.9}, glm::vec2{9,10});
    auto obstacle_block_pos = GENERATE(glm::ivec3{5, 4, 9}, glm::ivec3{6, 8, 9});

    CraftCpp::Chunk c{pq_coords};
    // empty chunk
    REQUIRE(!c.isLocalBufferReady());
    REQUIRE(c.getBlock(obstacle_block_pos) == CraftCpp::BlockType::EMPTY);

    c.setBlock(obstacle_block_pos, CraftCpp::BlockType::SAND);
    REQUIRE(c.getBlock(obstacle_block_pos) == CraftCpp::BlockType::SAND);

    c.setBlock(obstacle_block_pos, CraftCpp::BlockType::EMPTY);
    REQUIRE(c.getBlock(obstacle_block_pos) == CraftCpp::BlockType::EMPTY);

    c.setBlock(obstacle_block_pos, CraftCpp::BlockType::STONE);
    c.setBlock(obstacle_block_pos + glm::ivec3{2, 0, 0}, CraftCpp::BlockType::CLOUD);
    REQUIRE(c.getHighestBlock() == obstacle_block_pos.y);
}

TEST_CASE("Chunk instantiation with initialization"){
    glm::ivec2 pq_coords{2,2};
    std::forward_list<CraftCpp::Chunk> chunk_list{};

    for(int p_offset = 0 ; p_offset < 2 ; p_offset++){
        for(int q_offset = 0 ; q_offset < 2 ; q_offset++){
            chunk_list.emplace_front(pq_coords + glm::ivec2{p_offset, q_offset}).initChunk();
        }
    }
    for(const CraftCpp::Chunk &c : chunk_list) {
        c.waitThread();
        REQUIRE(c.isLocalBufferReady());
        REQUIRE(c.getHighestBlock() <= CraftCpp::Chunk::Y_LIMIT);
        for (int x = c.getMinX(); x <= c.getMaxX(); x++) {
            for (int z = c.getMinZ(); z <= c.getMaxZ(); z++) {
                bool empty_found = false;
                static constexpr int minHeight = 11;
                for(int y = minHeight ; y < CraftCpp::Chunk::Y_LIMIT ; y++) {
                    if (c.isOnBorder(glm::ivec3{x, y, z})) {
                        REQUIRE(c.getBlock({x, 0, z}) == CraftCpp::BlockType::EMPTY);
                        continue;
                    }
                    auto tb = CraftCpp::TileBlock{c.getBlock({x, y, z})};
                    empty_found = empty_found || tb.isEmpty();
                    bool assertion = (tb.isUserBuildable() && !empty_found) ||
                                     (!tb.isUserBuildable() && empty_found) || tb.getBlockType() == CraftCpp::BlockType::LEAVES;
                    REQUIRE(assertion);
                }
            }
        }
    }
}

TEST_CASE("Visibility formula"){
    glm::vec3 inner{0,0,1};
    glm::vec3 up_outer{0,1,1};
    glm::vec3 down_outer{0,-1,1};
    glm::vec3 left_outer{-10,0,1};
    glm::vec3 right_outer{1,0,1};

    auto p = glm::perspective(
            45.0f,
            800.f/600.f,
            .5f,
            2.f
    );
    auto v = glm::lookAt(
            glm::vec3{0.f,0.f,0.f},
            //glm::vec3{(c.get_min_x() + c.get_max_x()) / 2,c.getHighestBlock() / 2,(c.get_min_z() + c.get_max_z()) / 2},
            inner,
            glm::vec3{0.f,1.f,0.f}
    );

    glm::vec4 clip_inner = p * v * glm::vec4{inner, 1};
    glm::vec4 clip_up_outer = p * v * glm::vec4{up_outer, 1};
    glm::vec4 clip_down_outer = p * v * glm::vec4{down_outer, 1};
    glm::vec4 clip_left_outer = p * v * glm::vec4{left_outer, 1};
    glm::vec4 clip_right_outer = p * v * glm::vec4{right_outer, 1};

    clip_inner /= clip_inner.w;
    clip_up_outer /= clip_up_outer.w;
    clip_down_outer /= clip_down_outer.w;
    clip_left_outer /= -clip_left_outer.w;
    clip_right_outer /= -clip_right_outer.w;

    REQUIRE((glm::abs(clip_inner.x) < 1 && glm::abs(clip_inner.y) < 1 && glm::abs(clip_inner.z) < 1));
    REQUIRE((clip_up_outer.y > 1 && glm::abs(clip_up_outer.x) < 1 && glm::abs(clip_up_outer.z) < 1));
    REQUIRE((clip_down_outer.y < -1 && glm::abs(clip_down_outer.x) < 1 && glm::abs(clip_down_outer.z) < 1));
    REQUIRE((clip_left_outer.x < -1 && glm::abs(clip_left_outer.y) < 1 && glm::abs(clip_left_outer.z) < 1));
    REQUIRE((clip_right_outer.x > 1 && glm::abs(clip_right_outer.y) < 1 && glm::abs(clip_right_outer.z) < 1));
}
