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
};

template <typename T>
struct Vec<T,2>{
    T x;
    T y;
};

template <>
struct Vec<float,4>{
    float vertex1;
    float vertex2;
    float vertex3;
    float vertex4;
};
using Quad = Vec<float,4>;

template <>
struct Vec<int, 6>{
    int left;
    int right;
    int top;
    int bottom;
    int front;
    int back;
};
using Block = Vec<int, 6>;

#endif //CPP_DATA_STRUCTURES_HPP
