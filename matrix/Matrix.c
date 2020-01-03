//
// Created by alpharius on 27.12.19.
//

#include "Matrix.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#define DEBUG

static void _print();
static double _access(Matrix*, size_t, size_t);
static void _write(Matrix* , size_t ,size_t ,double);
static Matrix _T(Matrix*);
static void free_data(Matrix*);
static Matrix _minor(Matrix*, size_t, size_t);


Matrix new_Matrix(size_t height, size_t width){
    assert(height!= 0 && width!= 0);

    Matrix result;
    result.height = height;
    result.width = width;

    result.data = (double*) calloc(height*width, sizeof(double));
    result.PRINT = _print;
    result.ACCESS = _access;
    result.WRITE = _write;
    result.T = _T;
    result.MINOR = _minor;

    return result;
}

static void _print(Matrix* M){
    assert(M != NULL);
    for(size_t i=0; i<M->height; i++){
        for(size_t j=0; j<M->width; j++){
            printf("|%f \t", M->ACCESS(M, i, j));
        }
        printf("|\n");
    }

}

static double _access(Matrix* M, size_t i, size_t j){
    assert(M != NULL);
    assert(i < M->height);
    assert(j < M->width);

    return *(M->data + i*M->width + j);
}

static void _write(Matrix* M, size_t i, size_t j, double input){
    assert( M != NULL);
    assert(i < M->height);
    assert(j < M->width);

    *(M->data + i*M->width + j) = input;
}

// просто операция транспонирования
static Matrix _T(Matrix* M){
    Matrix result = new_Matrix(M->width, M->height);
    for(size_t i=0; i<M->height; i++){
        for(size_t j=0; j<M->width; j++){
            result.WRITE(&result, j, i, M->ACCESS(M, i, j));
        }
    }
    free_data(M);
    return result;
}

// освобождение данных - моя попытка хотябы как то избежать утечки памяти
// хотя она и будет - с учетом того, что я не могу удалить переменные отвечающие за размерность матрицы
static void free_data(Matrix* M){
    free(M->data);
    M = NULL;
}

// возвращаем матрицу - минор для переданной матрицы.
static Matrix _minor(Matrix* M, size_t I, size_t J){
    assert(M != NULL);
    assert(M->height != 1 && M->width != 1); // иначе отыскание минора не имеет смысла.
    Matrix result = new_Matrix(M->height-1, M->width-1);
    for(size_t i=0; i<M->height; i++){
        for(size_t j=0; j<M->width; j++){
            // использована неоптимальная форма
            if(i < I && j < J){
                result.WRITE(&result, i, j, M->ACCESS(M, i, j));
            }
            if (i > I && j < J){
                result.WRITE(&result, i-1, j, M->ACCESS(M, i, j));
            }
            if (i < I && j > J){
                result.WRITE(&result, i, j-1, M->ACCESS(M, i, j));
            }
            if (i > I && j > J){
                result.WRITE(&result, i-1, j-1, M->ACCESS(M, i, j));
            }
//            if (i != I || j != J){
//                // использована оптимальная форма
//                result.WRITE(&result, i-1*((int) (i>I)), j-1*((int) (j>J)) , M->ACCESS(M, i, j));
//            }

        }
    }
    return result;
}






    