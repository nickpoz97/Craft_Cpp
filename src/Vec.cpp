//
// Created by ultimatenick on 31/07/21.
//
#include "Vec.hpp"
#include "cmath"

template<typename T>
Vec<T, 3> operator+(const Vec<T, 3> &a, const Vec<T, 3> &b) {
    return {
            a.x + b.x,
            a.y + b.y,
            a.z + b.z
    };
}

template<typename T>
Vec<T, 3> operator*(T a, const Vec<T, 3> &b) {
    return {
            a * b.x,
            a * b.y,
            a * b.z
    };
}

template<typename T>
Vec<T, 3>& Vec<T, 3>::operator/=(T a) {
    x /= a;
    y /= a;
    z /= a;

    return *this;
}

template<typename T>
Vec<T, 3> Vec<T, 3>::normalize() {
    *this /= norm();
    return *this;
}

template<typename T>
T Vec<T, 3>::norm() const {
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

Vec<float, 4> Vec<float,4>::normalize() {
    x /= norm();
    y /= norm();
    z /= norm();

    return *this;
}

float Vec<float, 4>::norm() const {
    return Vec<float,3>{x,y,z}.norm();
}

float operator*(const Vec<float, 3> &a, const Vec<float, 3> &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
bool Vec<T, 3>::operator==(const Vec<T, 3> &other) const{
    return x == other.x &&
        y == other.y &&
        z == other.z;
}

template<typename T>
Vec<T, 3> operator-(const Vec<T, 3> &a, const Vec<T, 3> &b) {
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}
