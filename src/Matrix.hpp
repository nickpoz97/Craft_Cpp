//
// Created by ultimatenick on 07/08/21.
//

#ifndef CPP_MATRIX_HPP
#define CPP_MATRIX_HPP

#include "Vec.hpp"

class Matrix{
private:
    float elements[4][4];

public:
    Matrix();
    [[nodiscard]] Matrix transpose() const;

    void gen_traslation_matrix(Vec<float, 3> position);
    void generate_rotation_matrix(Vec<float, 3> axis, float radians_angle); // general rotation

    Vec<float,4> operator*(Vec<float,4> v) const;
    Matrix operator*(const Matrix& m) const;
};


#endif //CPP_MATRIX_HPP
