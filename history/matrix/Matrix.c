//
// Created by alpharius on 27.12.19.
//

#include "Matrix.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Matrix_functions.h"

#define ISODD(n)    (((n)%2 != 0) ? 1 : 0)
#define SIGN(n)     (ISODD(n) ? -1 : 1);

/*
 *  методы структуры матрицы - принимают сам же объект этой структуры в качестве своего парамета.
 *  то что в C++ является как this или self в питоне - указатель на экземпляр класса.
 */

static void _print();
static double _access(Matrix*, size_t, size_t);
static void _write(Matrix* , size_t ,size_t ,double);
static Matrix _T(Matrix*);
static void free_data(Matrix*);
static Matrix _minor(Matrix*, size_t, size_t);
static Matrix _inverse(Matrix*);
static Matrix* _row_op(Matrix*, size_t, size_t, double);
static Matrix* _swap_two_rows(Matrix*, size_t, size_t);

// "конструктор" объекта структуры матриа.
Matrix new_Matrix(size_t height, size_t width){
    assert(height!= 0 && width!= 0);

    // создание объекта и провозглашение его численных параметров и аллокация памяти
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
    result.SWAP = _swap_two_rows;

    return result;
}

// печать содержимого матрицы - переданной по указателю
static void _print(Matrix* M){
    assert(M != NULL);
    for(size_t i=0; i<M->height; i++){
        for(size_t j=0; j<M->width; j++){
            printf("|%lf \t", M->ACCESS(M, i, j));
        }
        printf("|\n");
    }
    printf("\n");
}

// получение хранящегося в матрице значения, по конкретной строке и столбцу
static double _access(Matrix* M, size_t i, size_t j){
    // то что в многомерном массиве бы записалось как A[i][j] тут описывалось бы по другому.
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

// просто операция транспонирования - проверена добротная
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

// освобождение данных
static void free_data(Matrix* M){
    // хотя она (утечка) и будет - с учетом того, что я не могу удалить переменные отвечающие за размерность матрицы
    free(M->data);
    M = NULL;
}

// возвращаем матрицу - минор для переданной матрицы.
static Matrix _minor(Matrix* M, size_t I, size_t J){
    /*
     *  задача состоит в возвращении матрицы, в которой бы отсутствовала определенная строка и столбец
     *  (второй и третий аргументы) в дальнейшем эта матрица будет передана на det() для вычисления
     *  матричного минора (отределитель минорной матрицы), служебная функция для отыскания обратной
     *  матрицы. Верно реализована.
     */

    assert(M != NULL);
    assert(M->height != 1 && M->width != 1); // иначе отыскание минора не имеет смысла.
    Matrix result = new_Matrix(M->height-1, M->width-1);
    for(size_t i=0; i<M->height; i++){
        for(size_t j=0; j<M->width; j++){
            /*  памятник тому почему лучше использовать оптимальную форму
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
            */
            // реализация оптимальной формы заполнения минорной матрицы
            if (i != I && j != J){
                result.WRITE(&result, i-1*((int) (i>I)), j-1*((int) (j>J)) , M->ACCESS(M, i, j));
            }

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
     *   ради экономии места - операцию транспонирования я проведу на этапе записи [1]
     *
     *   возможно, что алгоритм не оптимален - но формально точен - по определению из линала
     *   проверил на произвольной матрице.
     */
    assert(M != NULL);
    assert(M->height == M->width);

    double _det = det(M);
    assert(_det != 0.); // возможно тут стоит ввести точность - для плохо обусловленных матриц EPSILON ?
    int sign = 1; // это для знаков - начинается с + а дальше знакочередуется

    Matrix result = new_Matrix(M->height, M->width);
    for(size_t i=0; i<M->height; i++){
        for(size_t j=0; j<M->width; j++){
            Matrix minor_i_j = M->MINOR(M, i, j);               // [1]
            sign = SIGN(i+j);
            result.WRITE(&result, j, i, sign/_det * det(&minor_i_j)); //[1] провожу запись уже транспонированнх значений
        }
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
     *  NB - хотя это и дискуссионный вопрос - посоветоваться бы с опытными товарищами
     *
     *  проверил - численные операции над строками не меняют определителя - годная проверка корректности.
     *
     *  порядок аргументов вытекает из логики метода Гаусса.
     *
     *  Алгоритм свапа двух строк:
     *      M1.ROW_OP(&M1, 0, 1, 1);
     *      M1.ROW_OP(&M1, 1, 0, -1);
     *      M1.ROW_OP(&M1, 0, 1, 1);
     *      M1.ROW_OP(&M1, 0, 0, -2);
     * просто проводишь запись строк r1 & r2 и проделываешь с ними классическую операцию a+b & a-b
     * a' & b' и обратное восстановление как полусумму и как полуразность.
     */

    assert(M != NULL);
    assert(M->height != 1);
    for(size_t j=0; j<M->width; j++){
        double modify_value = M->ACCESS(M, second, j) + coef * M->ACCESS(M, first, j);
        M->WRITE(M, second, j, modify_value);
    }
    return M;
}

// реализация алгоритма перестановки двух строчек
static Matrix* _swap_two_rows(Matrix* M, size_t first, size_t second){
    /*
    *  Алгоритм свапа двух строк:
    *      M1.ROW_OP(&M1, 0, 1, 1);
    *      M1.ROW_OP(&M1, 1, 0, -1);
    *      M1.ROW_OP(&M1, 0, 1, 1);
    *      M1.ROW_OP(&M1, 0, 0, -2);
    * просто проводишь запись строк r1 & r2 и проделываешь с ними классическую операцию a+b & a-b
    * a' & b' и обратное восстановление как полусумму и как полуразность.
    */
    M->ROW_OP(M, first, second, 1);
    M->ROW_OP(M, second, first, -1);
    M->ROW_OP(M, first, second, 1);
    M->ROW_OP(M, first, first, -2);
    return M;
}


#undef SIGN
#undef ISODD
