//
// Created by ultimatenick on 28/09/21.
//

#include "catch2/catch.hpp"
#include "Geometry/map_utils.hpp"
#include "Geometry/BlockObject.hpp"

TEST_CASE("BlockObject instantiation"){
    SECTION("Cube instantiation, all faces"){
        std::vector<CraftCpp::CubeVertex> buffer{CraftCpp::Cube::maxIndices};
        CraftCpp::Cube c{CraftCpp::BlockType::SAND, {1, 1, 1, 1, 1, 1}, {2, 4, 5}, buffer.begin(), {}};
        REQUIRE(buffer.begin() == c.begin());
        REQUIRE(buffer.end() == c.end());
        REQUIRE(CraftCpp::Cube::maxIndices == 6 * 6);
    }
    SECTION("Plant instantiation, all faces"){
        std::vector<CraftCpp::CubeVertex> buffer{CraftCpp::Plant::maxIndices};
        CraftCpp::Cube c{CraftCpp::BlockType::RED_FLOWER, {1, 1, 1, 1}, {2, 4, 5}, buffer.begin(), {}};
        REQUIRE(buffer.begin() == c.begin());
        REQUIRE(buffer.end() == c.end());
        REQUIRE(CraftCpp::Plant::maxIndices == 6 * 4);
    }
    SECTION("Cube instantiation, 2 faces"){
        std::vector<CraftCpp::CubeVertex> buffer{2 * CraftCpp::INDICES_FACE_COUNT};
        CraftCpp::Cube c{CraftCpp::BlockType::CEMENT, {1, 0, 0, 1, 0, 0}, {2, 4, 5}, buffer.begin(), {}};
        REQUIRE(buffer.begin() == c.begin());
        REQUIRE(buffer.end() == c.end());
    }
}
