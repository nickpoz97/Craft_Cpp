//
// Created by ultimatenick on 28/09/21.
//

#include <forward_list>
#include "Geometry/Chunk.hpp"
#include "catch.hpp"
#include "gtc/matrix_transform.hpp"

TEST_CASE("Chunk instantiation (not initialized) and block insertion", "[instantiation]"){
    auto pq_coords = GENERATE(glm::vec2{4.7,6.9}, glm::vec2{9,10});
    auto obstacle_block_pos = GENERATE(glm::ivec3{5, 4, 9}, glm::ivec3{6, 8, 9});

    Chunk c{pq_coords};
    // empty chunk
    REQUIRE(!c.isLocalBufferReady());
    REQUIRE(c.get_block(obstacle_block_pos) == BlockType::EMPTY);

    c.set_block(obstacle_block_pos, BlockType::SAND);
    REQUIRE(c.get_block(obstacle_block_pos) == BlockType::SAND);

    c.set_block(obstacle_block_pos, BlockType::EMPTY);
    REQUIRE(c.get_block(obstacle_block_pos) == BlockType::EMPTY);

    c.set_block(obstacle_block_pos, BlockType::STONE);
    c.set_block(obstacle_block_pos + glm::ivec3{2,0,0}, BlockType::CLOUD);
    REQUIRE(c.getHighestBlock() == obstacle_block_pos.y);
}

TEST_CASE("Chunk instantiation with initialization", "[instantiation][chunk_initialization]"){
    glm::ivec2 pq_coords{2,2};
    std::forward_list<Chunk> chunk_list{};

    for(int p_offset = 0 ; p_offset < 2 ; p_offset++){
        for(int q_offset = 0 ; q_offset < 2 ; q_offset++){
            chunk_list.emplace_front(pq_coords + glm::ivec2{p_offset, q_offset});
        }
    }
    for(const Chunk &c : chunk_list) {
        c.wait_thread();
        REQUIRE(c.isLocalBufferReady());
        REQUIRE(c.getHighestBlock() <= Chunk::get_y_limit());
        for (int x = c.get_min_x(); x <= c.get_max_x(); x++) {
            for (int z = c.get_min_z(); z <= c.get_max_z(); z++) {
                bool empty_found = false;
                for(int y = 0 ; y < Chunk::get_y_limit() ; y++) {
                    if (c.is_on_border(glm::ivec3{x,y,z})) {
                        REQUIRE(c.get_block({x, 0, z}) == BlockType::EMPTY);
                        continue;
                    }
                    auto tb = TileBlock{c.get_block({x, y, z})};
                    empty_found = empty_found || tb.is_empty();
                    bool assertion = (tb.is_user_buildable() && !empty_found) ||
                        (!tb.is_user_buildable() && empty_found);
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
