//
// Created by ultimatenick on 07/08/21.
//

#include <cmath>
#include <mat4x4.hpp>
#include <ext/matrix_transform.hpp>

#include "Player.hpp"
#include "Cube.hpp"
#include "costants.hpp"

decltype(Player::ao) Player::ao = {};
decltype(Player::light) Player::light = {{
    {0.8, 0.8, 0.8, 0.8},
    {0.8, 0.8, 0.8, 0.8},
    {0.8, 0.8, 0.8, 0.8},
    {0.8, 0.8, 0.8, 0.8},
    {0.8, 0.8, 0.8, 0.8},
    {0.8, 0.8, 0.8, 0.8}
}};
TileBlock Player::tiles{
    226, 224, 241, 209, 225, 227
};

Player::Player(const Model &model, std::string_view name, int id, const glm::vec3 &position, const glm::vec2 &rotation)
        :
    actual_status{position, rotation, glfwGetTime()},
    former_status1{actual_status},
    former_status2{actual_status},
    model{model},
    playerCube{tiles, ao, light},
    name{name},
    id{id}
    {

    glm::mat4 transform = glm::translate(glm::mat4{1.0f}, position);
    transform = glm::rotate(transform, rotation.y, {0, 1, 0});
    transform = glm::rotate(transform, rotation.x, {1, 0, 0});

    playerCube.apply_transform(transform);
}

glm::vec3 Player::get_motion_vector() {
    const glm::vec2& rotation = actual_status.rotation;

    if (!z_movement && !x_movement) {
        return {0,0,0};
    }
    float strafe = atan2f(z_movement, x_movement);
    if (model.is_flying()) {
        float m = cosf(rotation.x);
        float y = sinf(rotation.y);
        if (x_movement) {
            if (!z_movement) {
                y = 0;
            }
            m = 1;
        }
        if (z_movement > 0) {
            y = -y;
        }
        return {cosf(rotation.x + strafe) * m,
            y,
            sinf(rotation.x + strafe) * m
        };
    }
    else {
        return {cosf(rotation.x + strafe),
        0,
        sinf(rotation.x + strafe)};
    }
}

glm::vec3 Player::get_sight_vector() {
    const glm::vec2& rotation = actual_status.rotation;

    float m = cosf(rotation.y);
    return {
        cosf(rotation.x - glm::radians(90.0f)) * m,
        sinf(rotation.y),
        sinf(rotation.x - glm::radians(90.0f)) * m
    };
}

void Player::set_movement(int x, int z) {
    x_movement = x;
    z_movement = z;
}

void Player::draw() {
    playerCube.draw_triangles();
}

void Player::update_player(const glm::vec3 &new_position, const glm::vec2 &new_rotation, bool interpolate) {
    if(interpolate){
        former_status1 = former_status2;
        former_status2 = {new_position, new_rotation, glfwGetTime()};

        if (former_status2.rotation.x - former_status1.rotation.x > PI){
            former_status1.rotation.x += 2*PI;
        }

        if (former_status1.rotation.x - former_status2.rotation.x > PI){
            former_status1.rotation.x -= 2*PI;
        }
    }
    else{
        actual_status = {new_position, new_rotation, glfwGetTime()};
    }
}

void Player::interpolate_player() {
    double dt1 = former_status2.t - former_status1.t;
    double dt2 = glfwGetTime() - former_status2.t;

    dt1 = glm::min(dt1, 1.0);
    dt1 = glm::max(dt1, 0.1);

    auto p = static_cast<float>(glm::min(dt2/dt1, 1.0));
    update_player(former_status1 + (former_status2 - former_status1) * p, 0);
}

void Player::update_player(const Status& new_status, bool interpolate) {
    update_player(new_status.position, new_status.rotation, interpolate);
}

Status operator+(const Status &a, const Status &b) {
    return {
        a.position + b.position,
        a.rotation + b.rotation,
        0
    };
}

Status operator-(const Status &a, const Status &b) {
    return {
        a.position - b.position,
        a.rotation - b.rotation,
        0
    };
}

Status operator*(const Status &a, float p) {
    return {
        p * a.position,
        p * a.rotation,
        0
    };
}

