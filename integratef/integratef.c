//
// Created by alpharius on 28.07.19.
//

// делаем интегрирование функции одной переменной
// с возможностью выбора различных методов интегрирования

#include "integratef.h"





double integratef(double (*f)(double), struct integrate_parametrs* PARAM, enum MODS MD){
    double result = 0.;
    switch (MD){
        case QUADRATE:
            // не путать с TRAP разные методы, разные порядки точности
            goto quadrate_integration;
            break;
        case SIMPSON:
            goto simpson_integration;
            break;
        case TRAP:
            goto trap_integration;
            break;
        case MONTE_CARLO:
            goto monte_carlo_integration;
    }

#ifdef FLAT     // чтобы сжать
    //  интегрирование методом усредненный трапеций
    {
        quadrate_integration:;
        // введение параметров
        double ll = PARAM->left_limit;
        double rl = PARAM->right_limit;
        double dx;
        // шаг интегрирования - либо мы задаем
        if (PARAM->N == 0){
            dx = PARAM->dx;
        } else {
            dx = (rl - ll) / PARAM->N;
        }

        double x = ll;
        while(x<rl){
            result += f(x + dx/2) * dx; // усредненный прямоугольник
            x = x + dx;
        }
        return result;
    }

    {
        simpson_integration:;
        double ll = PARAM->left_limit;
        double rl = PARAM->right_limit;
        double dx;
        // шаг интегрирования - либо мы его вычисляем
        if (PARAM->N == 0){
            dx = PARAM->dx;
        } else {
            dx = (rl - ll) / PARAM->N;
        }

        double x = ll;
        while (x<rl){
            // TODO (в будущем) проверять на ошибки округления
            result += dx / 6 * ( f(x) + 4*f(x+dx/2) + f(x+dx));
            x += dx;
        }
    }

    {
        trap_integration:;
        double ll = PARAM->left_limit;
        double rl = PARAM->right_limit;
        double dx;
        if (PARAM->N == 0){
            dx = PARAM->dx;
        } else {
            dx = (rl - ll) / PARAM->N;
        }

        double x = ll;
        while (x<rl){
            result += (f(x) + f(x+dx))/2 * dx;
        }

    }
#endif
    {
        monte_carlo_integration:;
        double ll = PARAM->left_limit;
        double rl = PARAM->right_limit;
        double fun_sup = PARAM->function_supremum;
        double fun_inf = PARAM->function_infinum;
        double eps = 0;
        size_t  counter=0;
        double int_sum_deltha = 2.;
        double s1, s2;

        if (PARAM->N ==0){
            // означает что мы остановим интегрирование опираясь на дисперсионный критерий
            eps = PARAM->eps;
        }

        while (counter<PARAM->N || int_sum_deltha > eps){
            
        }
    }
}


#define NTEST
#ifdef TEST

static double g_L;
static int g_n;


double ff(double x){
    return x/g_L*sin(M_PI*g_n*x/g_L);
}
int main(void){


    // memset - позволит нам установить биты по умолчанию нулями - чтобы не было мусора в памяти
    struct integrate_parametrs ap;
    memset(&ap, 0, sizeof(ap));             // NB

    {
        ap.left_limit = -2.;
        ap.right_limit = 2.;
        ap.N = 500;
    }
    g_L  = 2;
    double res;// = integratef(ff, &ap, QUADRATE);
    for(g_n=1;g_n<10; g_n++){
        res = integratef(ff, &ap, QUADRATE);
        printf("%f16\n",res);
    }


    return 0;
}
#endif




