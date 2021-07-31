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
