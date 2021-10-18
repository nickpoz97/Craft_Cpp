//
// Created by ultimatenick on 28/09/21.
//

#include "catch.hpp"
#include "Geometry/CubicObject.hpp"

TEST_CASE("CubicObject instantiation", "[instantiation]"){
    SECTION("Cube instantiation, all faces"){
        std::vector<CubeVertex> buffer{Cube::max_indices};
        Cube c{BlockType::SAND, {1, 1, 1, 1, 1, 1}, {2, 4, 5}, buffer.begin()}
        (<#initializer#>, <#initializer#>,
                <#initializer#>, cube_vertex_iterator_t(), 0);
        REQUIRE(buffer.begin() == c.begin());
        REQUIRE(buffer.end() == c.end());
        REQUIRE(Cube::max_indices == 6 * 6);
    }
    SECTION("Plant instantiation, all faces"){
        std::vector<CubeVertex> buffer{Plant::max_indices};
        Cube c{BlockType::RED_FLOWER, {1, 1, 1, 1}, {2, 4, 5}, buffer.begin()}
        (<#initializer#>, <#initializer#>,
                <#initializer#>, cube_vertex_iterator_t(),
                0);
        REQUIRE(buffer.begin() == c.begin());
        REQUIRE(buffer.end() == c.end());
        REQUIRE(Plant::max_indices == 6 * 4);
    }
    SECTION("Cube instantiation, 2 faces"){
        std::vector<CubeVertex> buffer{2 * INDICES_FACE_COUNT};
        Cube c{BlockType::CEMENT, {1, 0, 0, 1, 0, 0}, {2, 4, 5}, buffer.begin()}
        (<#initializer#>, <#initializer#>,
                <#initializer#>, cube_vertex_iterator_t(),
                0);
        REQUIRE(buffer.begin() == c.begin());
        REQUIRE(buffer.end() == c.end());
    }
}
