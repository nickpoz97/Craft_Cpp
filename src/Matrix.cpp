//
// Created by ultimatenick on 07/08/21.
//

#include <cmath>

#include "Matrix.hpp"

Matrix::Matrix() : elements{
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}
} {};

void Matrix::gen_traslation_matrix(Vec<float, 3> position) {
    elements[0][3] = position.x;
    elements[1][3] = position.y;
    elements[2][3] = position.z;
}

void Matrix::generate_rotation_matrix(Vec<float, 3> axis, float radians_angle) {
    axis.normalize();
    float s = sin(radians_angle);
    float c = cos(radians_angle);
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

Vec<float,4> Matrix::operator*(Vec<float, 4> v) const{
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
