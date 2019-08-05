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
            goto trap_integration;
            break;
        case SIMPSON:
            break;
        case TRAP:
            break;
    }


    //  интегрирование методом усредненный трапеций
    {
        trap_integration:;
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




