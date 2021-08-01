//
// Created by ultimatenick on 30/07/21.
//

#ifndef CPP_DATA_STRUCTURES_HPP
#define CPP_DATA_STRUCTURES_HPP

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
    float ao[6][4];
    float light[6][4];

    friend class Cube;
};

#endif //CPP_DATA_STRUCTURES_HPP
