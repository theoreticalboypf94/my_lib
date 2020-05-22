//
// Created by alpharius on 06.08.19.
//

#include "interpolate.h"
//#include

#define TEST//NOT_TEST

// TODO - написать динамически линкуемую версию полинома лагранжа
// не оптимизированные полиномы Лагранжа.
double Lagrange_polinom_interpolation(double x, double* x_set, double* y_set, size_t len){
    double result =0;
    // промежуточные произведения
    double loop_result=1;
    for(size_t i=0; i<len; i++){
        for(size_t j=0; j<len; j++){
            if (i!=j)
                loop_result *= (x-x_set[j]) / (x_set[i]-x_set[j]);
        }
        result += y_set[i] * loop_result;
        loop_result = 1.;
    }

    return result;
}




#ifdef TEST2
#define SIZE 50

int main(void){
    double x[SIZE];
    double y[SIZE];

    for(int i=0; i<SIZE; i++){
        x[i] = 0 + 2*M_PI/SIZE * i;
        y[i] = sin(x[i]);
    }

    for (int i=0; i<5; i++){
        y[i] = sin(x[i]);
    }

    double dx = 0.01;
    double arg = 0.;
    FILE *fp = fopen("interpolation.txt", "w");
    FILE *fp2 = fopen("sin.txt", "w");
    while (arg < 2*M_PI+1){
        fprintf(fp,"%lf %lf\n",arg, Lagrange_polinom_interpolation(arg, x, y, SIZE));
        fprintf(fp2, "%lf %lf\n", arg, sin(arg));
        arg += dx;
    }
    fclose(fp);
    fclose(fp);
    return 0;
}

#endif
