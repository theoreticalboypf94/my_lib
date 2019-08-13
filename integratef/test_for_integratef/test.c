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
#define SIZE 4

double f0(double);
double f1(double);
double f2(double);
double f3(double);

typedef double (*fptr)(double); // указатель на фукции

fptr fun_arr[] = {f0, f1, f2, f3};
char* fun_name[] = {"e**(-x**2)", "cos(sin(x))", "e**cos(x)", "x**3"};
// посчитано на вольфрам-альфа
double analitical_results[] = {1.77245, 7.5292, 10.3669 ,0};

typedef struct integrate_parametrs t_IP;
static t_IP* g_ip;


void initialize_ip(struct integrate_parametrs* _ip, double left_limit,
        double right_limit, double dx, size_t N,
        double function_sumrime, double function_infinum){
    _ip->left_limit = left_limit;
    _ip->right_limit = right_limit;
    _ip->dx = dx;
    _ip->N = N;
    _ip->function_supremum = function_sumrime;
    _ip->function_infinum = function_infinum;
}

int main(void){

    printf("| function\t | original\t | QUAD\t\t | SIMPS\t | TRAP\t\t | MONTE-CARLO\t | 3/8\t\t | \n");
    g_ip = (t_IP*) malloc(sizeof(t_IP));
    initialize_ip(g_ip, -5, 5, 0, 1000, 1, 0);

    for(size_t i=0; i<SIZE; i++) {
        double or = analitical_results[i];
        double qu = integratef(fun_arr[i], g_ip, QUADRATE);
        double si = integratef(fun_arr[i], g_ip, SIMPSON);
        double tr = integratef(fun_arr[i], g_ip, TRAP);
        double mc = integratef(fun_arr[i], g_ip, MONTE_CARLO);
        double te = integratef(fun_arr[i], g_ip, THREE_EIGHT);
        printf("| %s\t | %lf\t | %lf\t | %lf\t | %lf\t | %lf\t | %lf\t |\n", fun_name[i], analitical_results[i], qu, si,
               tr, mc, te);
    }
    return 0;
}


double f0(double x){
    return exp(-x*x);
}

double f1(double x){
    return cos(sin(x));
}

double f2(double x){
    return exp(cos(x));
}

double f3(double x){
    return x*x*x;
}