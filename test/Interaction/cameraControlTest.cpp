//
// Created by ultimatenick on 01/11/21.
//
#define GLFW_INCLUDE_NONE

#include "Interaction/CameraControl.hpp"
#include "Interaction/Camera.hpp"
#include "catch2/catch.hpp"

TEST_CASE("Test CameraControl instantiation with Camera") {
  {
    CraftCpp::Camera cam{{}, {90.0f, 0.0f}};
    auto camCtrl = CraftCpp::CameraControl::setInstance(cam);

    // No GameView instance
    REQUIRE(!CraftCpp::CameraControl::isInstantiated());
    REQUIRE(!camCtrl);

    auto gameViewPtr = CraftCpp::GameView::setInstance(1, 1, 1);
    camCtrl = CraftCpp::CameraControl::setInstance(cam);
    REQUIRE(CraftCpp::CameraControl::isInstantiated());
    REQUIRE(camCtrl);
  }
  REQUIRE(!CraftCpp::CameraControl::isInstantiated());
}