//
// Created by ultimatenick on 07/08/21.
//

#include <cmath>

#include "Player.hpp"
#include "cube.hpp"

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

Player::Player(Vec<float, 3> position, float rx, float ry, std::string_view name, int id) : rx{rx}, ry{ry},
    position{position},
    playerCube{{0,0,0}, tiles, ao, light},
    name{name},
    id{id}
    {
    Matrix rotation_asx;
    rotation_asx.generate_rotation_matrix({1,0,0}, rx);

    Matrix rotation_asy;
    rotation_asy.generate_rotation_matrix({cosf(rx), 0, sinf(rx)}, -ry);

    Matrix traslation;
    traslation.gen_traslation_matrix(position);

    playerCube.apply_transform(traslation * rotation_asy * rotation_asx);
}
