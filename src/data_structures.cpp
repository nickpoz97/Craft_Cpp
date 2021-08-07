//
// Created by ultimatenick on 31/07/21.
//
#include "data_structures.hpp"
#include "cmath"

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

Vec<float, 3>& Vec<float, 3>::operator/=(float a) {
    x /= a;
    y /= a;
    z /= a;

    return *this;
}

void Vec<float, 3>::normalize() {
    *this /= norm();
}

float Vec<float, 3>::norm() const {
    return std::sqrt(
        x*x + y*y + z*z
    );
}

Vec<float, 4> operator+(const Vec<float, 4> &a, const Vec<float, 4> &b) {
    return Vec<float, 4>{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    };
}

float operator*(const Vec<float, 4> &a, const Vec<float, 4> &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vec<float, 4>::Vec(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} {}

Vec<float, 4>::Vec(const float (&ar)[4]) : Vec{ar[0], ar[1], ar[2], ar[3]} {}

Vec<float, 4>::Vec(Vec<float, 3> v, float w) : x{v.x}, y{v.y}, z{v.z}, w{w} {};

void Vec<float,4>::normalize() {
    x /= norm();
    y /= norm();
    z /= norm();
}

float Vec<float, 4>::norm() const {
    return Vec<float,3>{x,y,z}.norm();
}

float operator*(const Vec<float, 3> &a, const Vec<float, 3> &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
