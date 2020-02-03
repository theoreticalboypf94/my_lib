//
// Created by alpharius on 27.12.19.
//

#ifndef ALLOC_TEST_MATRIX_H
#define ALLOC_TEST_MATRIX_H

#include <glob.h>

/*
 * реализация матричной структуры.
 * она содержит в себе указатели на функции, привязка которых происходит в момент запуска "конструктора"
 *  new_Matrix - который принимает в себя размер матрицы, которую надо создать
 *
 * Все методы должны в качестве первого аргумента, принимать указатель на "объект" матричной структуры
 *
 */

typedef struct matrix {
    size_t height, width;                                               // количество строк и столбцов
    double *data;                                                       // указатель на память хранящую числа
    double (*ACCESS)(struct matrix*, size_t, size_t);                   // доступ по указанным индексам
    void  (*PRINT)(struct matrix*);                                     // распечатка значений матрицы
    void (*WRITE)(struct matrix*, size_t, size_t, double);              // запись конкретного значения по индексам
    struct matrix (*T)(struct matrix*);                                 // операция транспонирования над матрицей
    struct matrix (*MINOR)(struct matrix*, size_t, size_t);             // поиск минорной матрицы для данного индекса [1]
    struct matrix (*INVERSE)(struct matrix*);                           // поиск обратной матрицы
    struct matrix* (*ROW_OP)(struct matrix*, size_t, size_t, double);   // операция со строками  s1 + a*s2 [2]
    struct matrix* (*SWAP)(struct matrix*, size_t, size_t);             // операция перестановки двух строк
} Matrix;

Matrix new_Matrix(size_t,size_t);                                       // "конструктор" объекта типа структуры

/*
 * [1] - по определению минор - это определитель матрицы с вычеркнутой строкой и столбцом, конкретно тут мы возвраща
 * ем минорную матрицу, от которой и предстоит взять определитель det() (Matrix_functions.h)
 * [2] - для будущей реализации метода гаусса TODO в будущем
 */

#endif //ALLOC_TEST_MATRIX_H
