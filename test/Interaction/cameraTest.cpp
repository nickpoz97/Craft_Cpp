//
// Created by ultimatenick on 27/09/21.
//

#include "Interaction/Camera.hpp"
#include "catch2/catch.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/trigonometric.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <Geometry/Chunk.hpp>

TEST_CASE("Camera instantiation and movement", "[instantiation]") {
  auto position{GENERATE(glm::vec3{0, 20, 0}, glm::vec3{53, 14, 78})};
  auto orientation{GENERATE(glm::vec2{0, 0}, glm::vec2{180, -30})};
  CraftCpp::Camera cam{position, orientation, 5};

  SECTION("Instantiate using arbitrary position and rotation") {
    REQUIRE(cam.getPos() == position);
    REQUIRE(cam.getPq() == CraftCpp::Chunk::chunked(cam.getPos()));

    glm::vec3 direction{glm::cos(glm::radians(orientation[0])) *
                            glm::cos(glm::radians(orientation[1])),
                        glm::sin(glm::radians(orientation[1])),
                        glm::sin(glm::radians(orientation[0])) *
                            glm::cos(glm::radians(orientation[1]))};
    auto equality =
        glm::epsilonEqual(glm::normalize(direction), cam.getDirection(), 0.01f);
    REQUIRE((equality.x && equality.y && equality.z));
  }
  SECTION("Test Camera movement translation") {
    glm::vec3 newPos = cam.getPos() + cam.getDirection() * cam.getSpeed();
    cam.shiftFront();
    REQUIRE(newPos == cam.getPos());

    newPos = cam.getPos() + glm::normalize(glm::cross(cam.getDirection(),
                                                      CraftCpp::Camera::up)) *
                                cam.getSpeed();
    cam.shiftRight();
    REQUIRE(newPos == cam.getPos());

    newPos = cam.getPos() + CraftCpp::Camera::up * cam.getSpeed();
    cam.shiftUp();
    REQUIRE(newPos == cam.getPos());

    glm::vec3 translation{-1.0, 3.0, 0.5};
    newPos = cam.getPos() + translation;
    cam.shift(translation);
    REQUIRE(newPos == cam.getPos());
  }
  SECTION("Rotate above bounds") {
    glm::vec2 rotation{40.0f, 180.0f};
    float newYaw = cam.getYaw() + rotation.x;
    newYaw = (newYaw >= 360.0f) ? newYaw - 360.0f : newYaw;
    newYaw = (newYaw < 0.0f) ? newYaw + 360.0f : newYaw;

    cam.rotate(rotation);
    REQUIRE(cam.getPitch() == 89.0f);
    REQUIRE(glm::epsilonEqual(newYaw, cam.getYaw(), 0.01f));
  }
}
