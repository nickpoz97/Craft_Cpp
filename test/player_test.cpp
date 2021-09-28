//
// Created by ultimatenick on 27/09/21.
//

#include "catch.hpp"
#include "Interaction/Player.hpp"
#include "vec3.hpp"
#include "vec2.hpp"

TEST_CASE("Player instantiation", "[instantiation]")
{
    auto position{GENERATE(glm::vec3{},glm::vec3{53,14,78})};

    SECTION("Using arbitrary position and default rotation") {
        Player p{"name", 0, position, {}};
        REQUIRE(p.get_position() == position);
        REQUIRE(p.get_orientation_degrees() == glm::vec2{});
        REQUIRE(p.get_camera_direction_vector() == glm::vec3{1.0, 0.0, 0.0});
    }
    SECTION("Using arbitrary position and pointing to up-right"){
        auto orientation{GENERATE(glm::vec2{45,45}, glm::vec2{60,30}, glm::vec2{30,60})};
        Player p{"name", 0, position, orientation};
        REQUIRE(p.get_position() == position);
        REQUIRE(p.get_orientation_degrees() == orientation);
        REQUIRE(p.get_camera_direction_vector().x < 1.0f);
        REQUIRE(p.get_camera_direction_vector().x > 0.0f);
        REQUIRE(p.get_camera_direction_vector().y < 1.0);
        REQUIRE(p.get_camera_direction_vector().y > 0.0);
    }
    SECTION("Using arbitrary position and pointing to down-left"){
        auto orientation{glm::vec2{200,-45}};
        auto position{glm::vec3{20,20,20}};
        Player p{"name", 20, position, orientation};
        REQUIRE(p.get_position() == position);
        REQUIRE(p.get_orientation_degrees() == orientation);
        REQUIRE(p.get_camera_direction_vector().x > -1.0f);
        REQUIRE(p.get_camera_direction_vector().x < 0.0f);
        REQUIRE(p.get_camera_direction_vector().y > -1.0);
        REQUIRE(p.get_camera_direction_vector().y < 0.0);
    }
    SECTION("Using arbitrary position and rotation arguments above angle boundaries"){
        auto orientation_pairs = (
            std::pair{
                glm::vec2{Player::yaw_limit, 2*Player::pitch_limit + 45},
                glm::vec2{0, Player::pitch_limit}},
            std::pair{
                glm::vec2{2 * Player::yaw_limit + 10,
                -2 * Player::pitch_limit - 15}, glm::vec2{10, -Player::pitch_limit}}
        );
        auto position{glm::vec3{53,14,78}};
        Player p{"name", 0, position, orientation_pairs.first};
        REQUIRE(p.get_position() == glm::vec3{53,14,78});
        REQUIRE(p.get_orientation_degrees() == orientation_pairs.second);
    }
}
