//
// Created by alpharius on 11.08.19.
//
#include "../integratef.h"


/*
 * Это тестовый файл, показывает:
 *      а) применение integratef к различным функция
 *      б) проводит исследование корректности выданного результата
 *      в) дает учет ошибок (путем перекрестного сравнения наличных результатов с аналитическими
 *
 * Билд ров:
 *      gcc -c integratef.c
 *      gcc -lm test.c integratef.o
 */


double f0(double);

typedef double (*fptr)(double); // указатель на фукции

fptr fun_arr[] = {f0};
char* fun_name[] = {"e**(x**2)"};
double analitical_results[] = {0.};
struct integrate_parametrs* g_ip;

void initialize_ip(struct integrate_parametrs* _ip, double left_limit,
        double right_limit, double dx, size_t N,
        double function_sumrime, double function_infinum){
    _ip->left_limit = left_limit;
    _ip->right_limit = right_limit;
    _ip->dx = dx;
    _ip->function_supremum = function_sumrime;
    _ip->function_infinum = function_infinum;
}

int main(void){

    printf("| function\t | original\t | QUAD\t | SIMPS\t | TRAP\t | MC\t | TE\t | ");
    initialize_ip(g_ip, -5, 5, 0, 1000, 0, 0);
    double or = analitical_results[0];
    double qu = integratef(fun_arr[0], g_ip, QUADRATE);
    double si = integratef(fun_arr[0], g_ip, SIMPSON);
    double tr = integratef(fun_arr[0], g_ip, TRAP);
    double mc = integratef(fun_arr[0], g_ip, MONTE_CARLO);
    double te = integratef(fun_arr[0], g_ip, THREE_EIGHT);
    printf("| %s\t | %lf\t | %lf\t | %lf\t | %lf\t | %lf\t | %lf\t |",fun_name[0], analitical_results[0], qu, si, tr, mc, te);
    return 0;
}


double f0(double x){
    return x*x;
}