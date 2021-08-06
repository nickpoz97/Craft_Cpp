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

void Matrix::traslation(Vec<float, 3> position) {
    elements[0][3] = position.x;
    elements[1][3] = position.y;
    elements[2][3] = position.z;
}

void Matrix::rotation(Vec<float, 3> axis, float angle) {
    axis.normalize();
    float s = sin(angle);
    float c = cos(angle);
    float m = 1 - c;

    elements[0][0] = m * axis.x * axis.x + c;
    elements[0][1] = m * axis.x * axis.y - axis.z * s;
    elements[0][2] = m * axis.z * axis.x + axis.y * s;
    elements[0][3] = 0;

    elements[1][0] = m * axis.x * axis.y + axis.z * s;
    elements[1][1] = m * axis.y * axis.y + c;
    elements[1][2] = m * axis.y * axis.z - axis.x * s;
    elements[1][3] = 0;

    elements[2][0] = m * axis.z * axis.x - axis.y * s;
    elements[2][1] = m * axis.y * axis.z + axis.x * s;
    elements[2][2] = m * axis.z * axis.z + c;
    elements[2][3] = 0;

    elements[3][0] = 0;
    elements[3][1] = 0;
    elements[3][2] = 0;
    elements[3][3] = 1;
}

Vec<float,4> Matrix::operator*(Vec<float, 4> v) {
    return {
            Vec<float, 4>(elements[0]) * v,
            Vec<float, 4>(elements[1]) * v,
            Vec<float, 4>(elements[2]) * v,
            Vec<float, 4>(elements[3]) * v
    };
}

Matrix Matrix::operator*(const Matrix& m) const{
    Matrix m_product;
    Matrix tm = m.transpose();

    for(int i = 0 ; i < 4 ; i++){
        for(int j = 0 ; j < 4 ; j++){
            m_product.elements[i][j] = Vec<float,4>{elements[i]} * Vec<float,4>{tm.elements[j]};
        }
    }

    return m_product;
}

Matrix Matrix::transpose() const{
    Matrix t;

    for(int i = 0 ; i < 4 ; i++){
        for (int j = 0 ; j < 4 ; j++){
            t.elements[i][j] = this->elements[i][j];
        }
    }

    return t;
}

