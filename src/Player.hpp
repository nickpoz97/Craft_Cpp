//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_PLAYER_HPP
#define CPP_PLAYER_HPP

#include <string_view>
#include <vec3.hpp>

#include "Cube.hpp"
#include "Model.hpp"

class Player {
private:
    static float ao[6][4];
    static float light[6][4];
    static TileBlock tiles;

    Cube playerCube;
    const Model& model;

    // status
    glm::vec2 rotation;
    glm::vec3 position;

    int x_movement = 0;
    int z_movement = 0;

    float t;

    std::string_view name;
    int id;
public:
    Player(const Model& model, const glm::vec3& position, const glm::vec2& rotation, std::string_view name, int id);
    glm::vec3 get_sight_vector();
    glm::vec3 get_motion_vector();
    void set_movement(int x, int z);
};


#endif //CPP_PLAYER_HPP
