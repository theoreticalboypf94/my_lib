//
// Created by alpharius on 14.08.19.
//

#include "../fs.h"

/*
 *  Этот файл показывает:
 *      а) применение integratef к различным функциям
 *      б) проводит корректность работы функции - через измерение MSE
 *
 * Build:
 *      gcc test_fs.c ../fs.c ../../integratef/integratef.c -lm
 *
 * Выводы:
 *      1
 */

double f(double);


int main(void){
    size_t N = 50;
    struct integrate_parametrs ip = {0,1, 0,1000,0,0};
    double *f_coef = fourier_coef(f,N,&ip,SIMPSON);

}


double f(double x){
    return x*x*sin(x)*exp(-x);
}
