#include <stdio.h>
#include "integratef/integratef.h"
#include "fourier_series/fs.h"

double fun2(double x)
{
    return log(x);
}

// только для хоршего вывода кэффициентов ряда фурье
#define PRETTY_PRINT(ptr)\
    printf("Number of series members :%d\n", (int) ptr[0]);     \
    /* a_i from 0 to n*/                                        \
    for(int i=0; i<=ptr[0]; i++){                               \
        printf("a_%d :\t%f\n",i, ptr[1+i]);                     \
    }                                                           \
    /* b_i from 1 to n*/                                        \
    for(int i=1; i<=ptr[0]; i++){                               \
        printf("b_%d :\t%f\n", i, ptr[1+((int)ptr[0]) + i]);    \
    }

int main() {
    double a = 1.;
    double b = 3.;
    int N = 1000;
    double dx = (b-a)/N, x=a;

    FILE *fp;
    fp = fopen("../graph.txt", "w");

    for(int i=0; i<N; i++){
        fprintf(fp, "%f %f\n", x, fun2(x));
        x+= dx;
    }
    fclose(fp);

    // fourier
    FILE *fp_f;
    fp_f = fopen("../graph_f.txt", "w");
    // fourier
    struct integrate_parametrs ip;
    ip.left_limit = a;
    ip.right_limit = b;
    ip.N = 1000;
    double *KOEF = fourier_coef(fun2, 6, &ip, QUADRATE);
    x = a;
    PRETTY_PRINT(KOEF);
    for(int i=0; i<3*N; i++){
        fprintf(fp_f, "%f %f\n", x, __function_series(KOEF, x));
        x+= dx;
    }
    fclose(fp_f);

    return 0;
}