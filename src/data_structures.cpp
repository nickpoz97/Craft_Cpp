//
// Created by ultimatenick on 31/07/21.
//
#include "data_structures.hpp"

template <typename T>
T Vec<T,6>::operator[](int i) {
    int* address = &(this->left) + i;
    return *address;
}

template <typename T>
T Vec<T,4>::operator[](int i) {
    T* address = &(this->v1) + i;
    return *address;
}

template<typename T, typename P>
Vec<T,3> operator+(const Vec<T,3>& a, const Vec<P,3>& b){
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

template<typename T, typename P>
Vec<T,3> operator*(P c, const Vec<T,3>& b){
    return {
        c * b.x,
        c * b.y,
        c * b.z
    };
}
