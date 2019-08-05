//
// Created by alpharius on 28.07.19.
//

#ifndef FURIER_TRANSFORM_INTEGRATEF_H
#define FURIER_TRANSFORM_INTEGRATEF_H

#include <stdio.h>
#include <math.h>
#include <memory.h>

/*
 *  MODS - перечисление различных способов интегрирования функции
 *  поля:
 *      QUADRATE - интегрирование методом усейченных трапеций
 *      SIMPSON -
 *      TRAP -
 */
enum MODS {
    QUADRATE,
    SIMPSON,
    TRAP
};

/*
 *  integrate_parametrs - структура, компактно содержащая в себе данные необходимые для проведения интегрирования
 *
 *  поля:
 *      left_limit - левый предел интегрирования функции
 *      right_limit - правый предел интегрирования функции
 *      dx - шаг интегрирования функции
 *      N - число кусков на который мы разбиваем отрезок
 *
 * замечание:
 *      dx, N -  взаимно протеворечивые параметры, integratef требует только одного из них
 *      можно указать тот параметр, который нужет, а второй оставить с нулевым значением,
 *      integratef - учтет это и вычислит значение по наличным параметрам
 */
struct integrate_parametrs{
    double left_limit;
    double right_limit;
    double dx;
    size_t N;
};

/*
 *  integratef - находит интеграл функции
 *
 *  параметры:
 *      f - указатель на функцию одного аргумента
 *      ip - указател  на структуру integrate_parametrs, содержащую в себе параметры интегрирования
 *      md - способ численно интегрировать входные данные,
 *
 *  возвращает:
 *      double - значение площади фигуры, ограниченной функциональным графиком
 */
double integratef(double (*f)(double), struct integrate_parametrs* ip,enum MODS md);

#endif //FURIER_TRANSFORM_INTEGRATEF_H
