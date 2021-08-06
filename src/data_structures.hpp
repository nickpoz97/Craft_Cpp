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

    void normalize();
    [[nodiscard]] float norm() const;

    friend Vec<float,3> operator+(const Vec<float,3>& a, const Vec<float,3>& b);
    friend float operator*(const Vec<float,3>& a, const Vec<float,3>& b);
    friend Vec<float,3> operator*(float a, const Vec<float,3>& b);
    Vec<float,3>& operator/=(float a);
};

template <>
struct Vec<float, 4>{
    float x, y, z, w;

    Vec(float x, float y, float z, float w);
    explicit Vec(const float (&ar)[4]);

    void normalize();
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

enum class Face{LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK};

class Vertex{
private:
    Vec<float, 3> position;
    int index;
    Face face;
    Vec<float,2> uv;
    float ao;
    float light;

    friend class Cube;
    friend class Plant;
};

class Matrix{
private:
    float elements[4][4] {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
    };

public:
    void traslation(Vec<float, 3> position);
    void rotation(Vec<float, 3> axis, float angle); // general rotation
    Matrix transpose() const;

    Vec<float,4> operator*(Vec<float,4> v);
    Matrix operator*(const Matrix& m) const;
};

#endif //CPP_DATA_STRUCTURES_HPP