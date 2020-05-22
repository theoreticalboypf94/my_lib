//
// Created by alpharius on 10.08.19.
//

#include <stddef.h>
#include <stdio.h>
#include <math.h>

#ifndef FURIER_TRANSFORM_COUNT_SERIES_H
#define FURIER_TRANSFORM_COUNT_SERIES_H

/*
 *      планируется реализовать несколько функций подсчета рядов
 *      которые бы опционно выбирались функцией оберткой
 */


/*
 *  count_series - вычисляет ряд
 *
 *  поля:
 *      N - число членов ряда которые мы суммируем
 *      law_fun - аналитическое выражение для следующего слогаемого ряда
 *          например 1/k**2
 * возвращает:
 *      конечную сумму
 */
double count_series(size_t N, double (*law_fun)(size_t));


#endif //FURIER_TRANSFORM_COUNT_SERIES_H
