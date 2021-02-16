//
// Created by alpharius on 09.02.2021.
//

#ifndef MATHEMATICA_PROJECT_VECTOR_H
#define MATHEMATICA_PROJECT_VECTOR_H
#include <clocale>
#include <cstdlib>
#include <initializer_list>
#include <assert.h>

typedef char vector_orientation;

template <typename T>
class Vector {
public:
    int len;
    T *ptr = NULL;
    vector_orientation orientation;

    Vector(int);
    Vector();
    ~Vector();
    Vector(Vector<T>&);
    Vector(std::initializer_list<T> l);

    Vector<T> operator+(const Vector<T>&);    // element-vise addition
    T operator*(Vector<T>&);    // dot-product
    T& operator[](int);                  // access to element
    Vector<T> operator^(Vector<T>&);    // vector-product

    Vector<T>& operator=(Vector<T>&);    // equal symbol
};


#endif //MATHEMATICA_PROJECT_VECTOR_H

#include "Vector_implementation.h"