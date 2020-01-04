//
// Created by alpharius on 27.12.19.
//

#include "Matrix.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Matrix_functions.h"

#define SIGN(i,j)

/*
 *  методы структуры матрицы - принимают сам же объект этой структуры в качестве своего парамета.
 */


static void _print();
static double _access(Matrix*, size_t, size_t);
static void _write(Matrix* , size_t ,size_t ,double);
static Matrix _T(Matrix*);
static void free_data(Matrix*);
static Matrix _minor(Matrix*, size_t, size_t);
static Matrix _inverse(Matrix*);
static Matrix* _row_op(Matrix*, size_t, size_t, double);


Matrix new_Matrix(size_t height, size_t width){
    assert(height!= 0 && width!= 0);

    // создание объекта и провозглашение его численных параметров и выделение памяти
    Matrix result;
    result.height = height;
    result.width = width;
    result.data = (double*) calloc(height*width, sizeof(double));

    // добавление функциональных возможностей
    result.PRINT = _print;
    result.ACCESS = _access;
    result.WRITE = _write;
    result.T = _T;
    result.MINOR = _minor;
    result.INVERSE = _inverse;
    result.ROW_OP = _row_op;

    return result;
}

// печать содержимого матрицы - переданной по указателю
static void _print(Matrix* M){
    assert(M != NULL);
    for(size_t i=0; i<M->height; i++){
        for(size_t j=0; j<M->width; j++){
            printf("|%f \t", M->ACCESS(M, i, j));
        }
        printf("|\n");
    }
    printf("\n");
}

// получение хранящегося в матрице значения, по конкретной строке и столбцу
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

// нахождение обратной матрицы
static Matrix _inverse(Matrix* M){
    /*
     *   A**-1 = 1./det * A_ij^T
     *
     *   A_ij - матрица алгебраических дополнений, исходной матрицы
     *   ради экономии места - операцию транспонирования я проведу на этапе записи
     */
    assert(M != NULL);
    assert(M->height == M->width);

    double _det = det(M);
    assert(_det != 0.); // возможно тут стоит ввести точность - для плохо обусловленных матриц EPSILON ?
    int sign = 1; // это для знаков - начинается с + а дальше знакочередуется

    Matrix result = new_Matrix(M->height, M->width);
    for(size_t i=0; i<M->height; i++){
        for(size_t j=0; j<M->width; j++){
            Matrix minor_i_j = M->MINOR(M, i, j);
            result.WRITE(&result, j, i, sign/_det * det(&minor_i_j));
            sign *= -1;
        }
        sign *= -1;
    }
    return result;
}

// производим арифметические операции над строками матрицы.
static Matrix* _row_op(Matrix* M, size_t first, size_t second, double coef){
    /*
     *  производим строчную операцию над матрицей - тут учтено как сложение, так и вычитание матриц
     *
     *  first
     *  ...
     *  second
     *
     *  second' = second + coef * first - поэлеметно
     *
     *  в принципе эта функция - модернизирует значения переданного аргумента - так что нет
     *  нужды в возвращении указателя на память - но пока оставлю эту возможность для будущих возможных потреблений
     *
     *  проверил - численные операции над строками не меняют определителя - годная проверка корректности.
     */

    assert(M != NULL);
    assert(M->height != 1);
    for(size_t j=0; j<M->width; j++){
        double modify_value = M->ACCESS(M, second, j) + coef * M->ACCESS(M, first, j);
        printf("%f \n", coef);
        M->WRITE(M, second, j, modify_value);
    }
    return M;
}

