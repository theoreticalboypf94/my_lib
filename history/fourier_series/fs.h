//
// Created by alpharius on 28.07.19.
//

#ifndef FURIER_TRANSFORM_FS_H
#define FURIER_TRANSFORM_FS_H

#include "../integratef/integratef.h"

#include <malloc.h>

/*
 * fourier_coef - функция возвращающая коэффициенты ряда фурье по формуле
 *
 * f(x) [-L, +L]
 * f(x) ~ a_0/ 2 + Sum_n { a_n * cos(pi * n * x / L) + b_n * sin( pi * n * x / L);
 * a_n = 1/L * Integrate from -L to L  {f(x) * cos(pi * n * x / L) dx
 * b_n = 1/L * Integrate from -L to L  {f(x) * sin(pi * n * x / L) dx
 *
 * параметры:
 *      f - указатель на функцию одного аргумента
 *      N - число членов ряда фурье
 *      ip - указатель на integrate_parametrs содержащий параметры интегрирования
 *      md - способ интегрирования
 *
 * возвращает:
 *      указатель на коэффициенты ряда фурье [n, a_0, a_1, ... , a_n, b_1, ... b_n]  b_0  == 0. (очевидно)
 *      всего будет 1+n+1+n = 2n + 2 элементов - 1й указывает на размер возвращаемого массива
 * замечание:
 *      внутри себя использует функцию  integratef - последние два параметра как раз для нее
 *
 *      не считаю нужным заводить дополнительную структуру для коэффициентов фурье - наличного указателя
 *      на алоцированную память хватает
*/
double* fourier_coef(double (*f)(double), int N , struct integrate_parametrs* ip, enum MODS md);


/*
 *  __function_series() - функция считающая значение функционального ряда (ФР.) для заданных коэффициентов фурье
 *
 *  параметры:
 *      koef - указатель на коэффициенты фурье (данные упакованны так как в fourier_coef)
 *          [n, a_0, a_1, ... , a_n, b_1, ... b_n], где n
 *
 *      x - точка в которой мы ищем значение ФР.
 *  возвращает:
 *      сумму значений ФР. приближение к функции-образу
 */
double  __function_series(double *koef, double x);

#endif //FURIER_TRANSFORM_FS_H
