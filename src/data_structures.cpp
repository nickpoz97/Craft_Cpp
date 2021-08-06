//
// Created by ultimatenick on 31/07/21.
//
#include "data_structures.hpp"

int TileBlock::operator[](int i) const{
    const int* address = &(this->leftFace) + i;
    return *address;
}

Vec<float, 3> operator+(const Vec<float, 3> &a, const Vec<float, 3> &b) {
    return {
            a.x + b.x,
            a.y + b.y,
            a.z + b.z
    };
}

Vec<float, 3> operator*(float a, const Vec<float, 3> &b) {
    return {
            a * b.x,
            a * b.y,
            a * b.z
    };
}

Matrix::Matrix() {}

void Matrix::translate(Vec<float, 3> position) {
    elements[0][3] = position.x;
    elements[1][3] = position.y;
    elements[2][3] = position.z;
}
