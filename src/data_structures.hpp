//
// Created by ultimatenick on 30/07/21.
//

#ifndef CPP_DATA_STRUCTURES_HPP
#define CPP_DATA_STRUCTURES_HPP
template <typename T, int n_components>
struct Vec;

template <typename T>
struct Vec<T,3>{
    T x;
    T y;
    T z;

    friend Vec<T,3> operator+(const Vec<T,3>& a, const Vec<T,3>& b);

    template<typename P>
    friend Vec<T,3> operator*(P a, const Vec<T,3>& b);
};

template <typename T>
struct Vec<T,2>{
    T x;
    T y;
};

// v stands for vertex
// Quad is a face of a cube
template <typename T>
struct Vec<T,4>{
    T v1;
    T v2;
    T v3;
    T v4;

    T operator[](int i);
};
using Quad = Vec<float,4>;
using Face = Vec<Vec<int,3>, 4>;

// specialization for cube faces
template <typename T>
struct Vec<T, 6>{
    T left;
    T right;
    T top;
    T bottom;
    T front;
    T back;

    T operator[](int i);
};
using Block = Vec<int, 6>;

#endif //CPP_DATA_STRUCTURES_HPP
