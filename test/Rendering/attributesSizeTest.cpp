//
// Created by ultimatenick on 04/11/21.
//

#include <Geometry/Vertex.hpp>
#include "Rendering/attributesSize.hpp"
#include "catch2/catch.hpp"

TEST_CASE("Test attributes sizes") {
    REQUIRE(CraftCpp::getPosAttributeSize<CraftCpp::Standard2DVertex>() == 2);
    REQUIRE(CraftCpp::getNormalAttributeSize<CraftCpp::Standard2DVertex>() == 0);
    REQUIRE(CraftCpp::getUvAttributeSize<CraftCpp::Standard2DVertex>() == 0);
    REQUIRE(CraftCpp::getAoAttributeSize<CraftCpp::Standard2DVertex>() == 0);

    REQUIRE(CraftCpp::getPosAttributeSize<CraftCpp::Uv2DVertex>() == 2);
    REQUIRE(CraftCpp::getNormalAttributeSize<CraftCpp::Uv2DVertex>() == 0);
    REQUIRE(CraftCpp::getUvAttributeSize<CraftCpp::Uv2DVertex>() == 2);
    REQUIRE(CraftCpp::getAoAttributeSize<CraftCpp::Uv2DVertex>() == 0);

    REQUIRE(CraftCpp::getPosAttributeSize<CraftCpp::Standard3DVertex>() == 3);
    REQUIRE(CraftCpp::getNormalAttributeSize<CraftCpp::Standard3DVertex>() == 0);
    REQUIRE(CraftCpp::getUvAttributeSize<CraftCpp::Standard3DVertex>() == 0);
    REQUIRE(CraftCpp::getAoAttributeSize<CraftCpp::Standard3DVertex>() == 0);

    REQUIRE(CraftCpp::getPosAttributeSize<CraftCpp::NormalVertex>() == 3);
    REQUIRE(CraftCpp::getNormalAttributeSize<CraftCpp::NormalVertex>() == 3);
    REQUIRE(CraftCpp::getUvAttributeSize<CraftCpp::NormalVertex>() == 2);
    REQUIRE(CraftCpp::getAoAttributeSize<CraftCpp::NormalVertex>() == 0);

    REQUIRE(CraftCpp::getPosAttributeSize<CraftCpp::Uv3DVertex>() == 3);
    REQUIRE(CraftCpp::getNormalAttributeSize<CraftCpp::Uv3DVertex>() == 0);
    REQUIRE(CraftCpp::getUvAttributeSize<CraftCpp::Uv3DVertex>() == 2);
    REQUIRE(CraftCpp::getAoAttributeSize<CraftCpp::Uv3DVertex>() == 0);

    REQUIRE(CraftCpp::getPosAttributeSize<CraftCpp::BlockVertex>() == 3);
    REQUIRE(CraftCpp::getNormalAttributeSize<CraftCpp::BlockVertex>() == 3);
    REQUIRE(CraftCpp::getUvAttributeSize<CraftCpp::BlockVertex>() == 2);
    REQUIRE(CraftCpp::getAoAttributeSize<CraftCpp::BlockVertex>() == 1);
}

