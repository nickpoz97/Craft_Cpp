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

    friend Vec<float,3> operator+(const Vec<float,3>& a, const Vec<float,3>& b);
    friend Vec<float,3> operator*(float a, const Vec<float,3>& b);
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

    void operator*(Vec<float,3> v);
    void operator*(Matrix m);
public:
    explicit Matrix();
    void translate(Vec<float, 3> position);
    void mat_rotate(Vec<float, 3> axis, float angle);
};

#endif //CPP_DATA_STRUCTURES_HPP
