//
// Created by alpharius on 16.02.2021.
//

#ifndef MATHEMATICA_PROJECT_MATRIX_H
#define MATHEMATICA_PROJECT_MATRIX_H

#include <initializer_list>
#include <cstddef>
#include <iostream>

#define SIGN(power)  (((power) % 2 == 0) ? 1 : -1)

template < typename G>
class Matrix{
private:
    G **m;
    size_t h, w;

    bool check_size(const Matrix<G>& another);          // проверка совпадения размеров двух матриц
    bool is_multiplicable(const Matrix<G>& another);    // проверка возможности перемножить матрицы  //TODO - не доделал
    bool is_square();                                   // проверка того что матрица квадратная
public:
    /* constructor and destructor */
    Matrix(size_t _h, size_t _w): h(_h) , w(_w){
        m = (G**) new G*[h];
        for(int i=0; i<h; ++i)
            m[i] = new G[w];
    }
    Matrix(Matrix& another) : Matrix(another.get_Height(), another.get_Width()){
        for(int i=0; i<another.get_Height(); i++)
            for(int j=0; j<another.get_Width(); j++)
                (*this)[i][j] = another[i][j];
    }
    Matrix(const std::initializer_list<std::initializer_list<G>>& lst) :
    Matrix(lst.size(),lst.begin()->size()){
        size_t row = 0;
        for(auto it = lst.begin(); it != lst.end(); it++)
            std::copy(it->begin(), it->end(), (*this)[row++]);
    }
    ~Matrix(){
        for(int i=0; i<h; ++i)
            delete[](m[i]);
        delete[](m);
    }

    /* internal methods */
    size_t get_Height() {return h;}
    size_t get_Width() {return w;}

    /* operator overloading */
    G* operator[](size_t row);
    Matrix<G>& operator+(Matrix<G>& another);
    Matrix<G>& operator^(int power);
    Matrix<G>& operator/(G devisor);
    Matrix<G>&operator*(Matrix<G>& another);

    /* det and transposition */
    template <typename U>
    friend U det(const Matrix<U>& m);
    template <typename U>
    friend Matrix<U> Minor(const Matrix<U>&m, size_t i, size_t j);
    Matrix<G>& T();

    /* output operator, to std::cout */
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);
};

#include "Matrix_implementation.h"

#endif //MATHEMATICA_PROJECT_MATRIX_H
