//
// Created by alpharius on 31.12.19.
//

#include <assert.h>
#include <stdio.h>
#include "Matrix_functions.h"



static double _matrix_functions_int_power(double,size_t);

// ищем определитель матрицы
double det(Matrix* M){
    /*
     *  это первый вариант определителя - неоптимальный
     *  раскладываем по первой строке
     *
     *  оптимизацией тут является просмотр - и разложение по строке-столбцу в котором наибольшее число нулей
     *  либо свершение операций над строками, столбцами.
     */
    double determinant = 0;
    assert(M != NULL);
    assert(M->width == M->height);

    if (M->width == 1){
        return M->ACCESS(M, 0, 0);
    }

    for(size_t j=0; j<M->width; j++){
        Matrix M_0_j = M->MINOR(M, 0, j);
        determinant += _matrix_functions_int_power(-1., j) * det(&M_0_j);

#ifdef DEBUG
        M_0_j.PRINT(&M_0_j);
        printf("\n ");
#endif
    }

    return determinant;
}


static double _matrix_functions_int_power(double base, size_t power){
    if (power == 0) return 1.;

    double result = base;
    for (size_t k=1; k<power; k++){
        result = result * base;
    }
    return result;
}