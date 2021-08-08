//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_PLAYER_HPP
#define CPP_PLAYER_HPP

#include "data_structures.hpp"
#include "cube.hpp"

class Player {
private:
    static float ao[6][4];
    static float light[6][4];
    static TileBlock tiles;

    Cube playerCube;
    float rx;
    float ry;

public:
    Player(Vec<float,3> position, float rx, float ry);
};


#endif //CPP_PLAYER_HPP
