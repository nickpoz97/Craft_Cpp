//
// Created by ultimatenick on 30/07/21.
//

#ifndef CPP_DATA_STRUCTURES_HPP
#define CPP_DATA_STRUCTURES_HPP

#include <array>

template <typename T, int n_components>
struct Vec;

template <typename T>
struct Vec<T,2>{
    T x;
    T y;
};

template <>
struct Vec<float,3>{
    float x;
    float y;
    float z;

    Vec<float, 3> normalize();
    [[nodiscard]] float norm() const;

    friend Vec<float,3> operator+(const Vec<float,3>& a, const Vec<float,3>& b);
    friend float operator*(const Vec<float,3>& a, const Vec<float,3>& b);
    friend Vec<float,3> operator*(float a, const Vec<float,3>& b);
    Vec<float,3>& operator/=(float a);
};

template <>
struct Vec<float, 4>{
    float x, y, z, w;

    Vec() = default;
    Vec(float x, float y, float z, float w);
    Vec(const float (&ar)[4]);
    Vec(Vec<float,3> v, float w);

    Vec<float, 4> normalize();
    [[nodiscard]] float norm() const;

    friend Vec<float,4> operator+(const Vec<float,4>& a, const Vec<float,4>& b);
    friend float operator*(const Vec<float,4>& a, const Vec<float,4>& b);
};

struct TileBlock{
    int leftFace;
    int rightFace;
    int topFace;
    int bottomFace;
    int frontFace;
    int backFace;

    int operator[](int i) const;
};

#endif //CPP_DATA_STRUCTURES_HPP
