//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_PLAYER_HPP
#define CPP_PLAYER_HPP

#include <string_view>

#include "Vec.hpp"
#include "cube.hpp"

class Player {
private:
    static float ao[6][4];
    static float light[6][4];
    static TileBlock tiles;

    Cube playerCube;

    float rx;
    float ry;
    Vec<float,3> position;

    float t;

    std::string_view name;
    int id;
public:
    Player(Vec<float,3> position, float rx, float ry, std::string_view name, int id);
};


#endif //CPP_PLAYER_HPP
