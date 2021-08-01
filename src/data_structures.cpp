//
// Created by ultimatenick on 31/07/21.
//
#include "data_structures.hpp"

int TileBlock::operator[](int i) const{
    const int* address = &(this->leftFace) + i;
    return *address;
}

Vec<float,3> Vec<Vec<float,3>,4>::operator[](int i) {
    Vec<float,3>* address = &(this->v1) + i;
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

Vec<float, 3> IndexedVertex::getNormal() {
    int faceIndex = static_cast<int>(face);

}

void IndexedVertex::setValues(int world_position_index, int face_index, Face face) {

}
