//
// Created by alpharius on 28.07.19.
//

// делаем интегрирование функции одной переменной
// с возможностью выбора различных методов интегрирования

#include "integratef.h"

#define INTEGRATEF_MONTE_CARLO_METHOD_ROBUST_VALUE 100


// uniform-random distribution
double get_random(void) { return ((double)rand() / (double)RAND_MAX); }


double integratef(double (*f)(double), struct integrate_parametrs* PARAM,
        enum MODS MD){
    double result = 0.;
    switch (MD){
        case QUADRATE:
            // не путать с TRAP разные методы, разные порядки точности
            goto quadrate_integration;
        case SIMPSON:
            goto simpson_integration;
        case TRAP:
            goto trap_integration;
        case MONTE_CARLO:
            goto monte_carlo_integration;
        case THREE_EIGHT:
            goto three_eight_integration;
    }

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

    {
        monte_carlo_integration:;
        srand(time(NULL));

        double ll = PARAM->left_limit;
        double rl = PARAM->right_limit;
        double fun_sup = PARAM->function_supremum;
        double fun_inf = PARAM->function_infinum;
        double eps = -1.;
        size_t  counter=0, internal1=0;
        double int_sum_deltha = 0.;
        double s1, s2;
        double compact_area = (rl - ll) * (fun_sup - fun_inf);
        double x,y;

        if (PARAM->N ==0){
            // означает что мы остановим интегрирование опираясь на дисперсионн
            // ый критерий
            // в противном случае, метод мк. закончит работу после сбора статис
            // тики по N точкам.
            // возможно стоит подернизировать метод и включить в рассмотрение
            // оба эти критерия одновременно
            eps = PARAM->eps;
        }

        // считаем либо до установенного предела либо до соседних флуктуаций ме
        // ньше заданной  тут сразу обобщение условий
        while (counter<PARAM->N && int_sum_deltha > eps){
            // ставим точку на компакт
            x = get_random() * (rl - ll) + ll;
            y = get_random() * (fun_sup - fun_inf) + fun_inf;

            // собираем статистику
            if (y < f(x) && y>0. && f(x)>0){
                internal1++;
            }

            if (y>f(x) && y<0 && f(x)<0){
                internal1++;
            }
            counter++;

            s1 = compact_area * ((double) internal1 / (double) counter );
            int_sum_deltha = fabs(s1 - s2);
            s2 = s1;

            // заглушка, для решения проблемы с флуктуациями
            // если 2 точки попали вне области функции - тогда будет 2 нуля
            // и остановка программы
            if (counter < INTEGRATEF_MONTE_CARLO_METHOD_ROBUST_VALUE){
                int_sum_deltha = 1.;
            }
        }
        result = s1;
        return result;
    }

    {
        three_eight_integration:;
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
        // проверить на корректность аргумента! (вроде так но может быть и
        // x , x+dx x+2dx x+3dx
        while (x<rl){
            result += 3./8. * (f(x) + 3*f(x+dx/3) + 3*f(x + 2./3 * dx) + f(x+dx))*dx;
            x += dx;
        }
        return result;
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




