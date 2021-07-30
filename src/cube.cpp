//
// Created by ultimatenick on 29/07/21.
//

#include "cube.hpp"
#include "item.hpp"

Cube::Cube(int nComponents, int nFaces) : data{new float[6 * nComponents * nFaces]} {}

Cube::~Cube(){
    delete[] data;
}

void Cube::makeCubeFaces(Block tiles, Block faces, Vec<float, 3> position,  Quad ao, Quad light, float n) {

}

