//
// Created by ultimatenick on 07/08/21.
//

#include <cmath>
#include <mat4x4.hpp>
#include <ext/matrix_transform.hpp>

#include "Player.hpp"
#include "Cube.hpp"

float Player::ao[6][4] = {0};
float Player::light[6][4] = {
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8},
        {0.8, 0.8, 0.8, 0.8}
};
TileBlock Player::tiles{
    226, 224, 241, 209, 225, 227
};

Player::Player(const Model& model, const glm::vec3& position, const glm::vec2& rotation, std::string_view name, int id) : rotation{rotation},
    model{model},
    position{position},
    playerCube{{0,0,0}, tiles, ao, light},
    name{name},
    id{id}
    {

    glm::mat4 transform = glm::translate(glm::mat4{1.0f}, position);
    transform = glm::rotate(transform, rotation.y, {0, 1, 0});
    transform = glm::rotate(transform, rotation.x, {1, 0, 0});

    playerCube.apply_transform(transform);
}

glm::vec3 Player::get_motion_vector() {
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

