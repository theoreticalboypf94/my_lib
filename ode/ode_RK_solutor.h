//
// Created by alpharius on 26.08.19.
//

#include <stddef.h>
#include <math.h>

#ifndef FURIER_TRANSFORM_ODE_RK_SOLUTOR_H
#define FURIER_TRANSFORM_ODE_RK_SOLUTOR_H


/*
 *  y^(n) + y^(n-1)*f_1 + ... +  y^(i)*f_n-i + ... + y'f_(n-1) + y*f_n + g = 0;
 *  y(init) = y_0
 *  y'(init) = y_1
 *
 */
typedef double (*funp)(double);
struct t_rk_parametrs {
    double from;
    double to;
    double h;
};
void ode_RK_solutor(size_t order, funp terms[], const struct t_rk_parametrs* options ,double* init_condition);

#endif //FURIER_TRANSFORM_ODE_RK_SOLUTOR_H
