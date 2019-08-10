//
// Created by alpharius on 10.08.19.
//


#include "count_series.h"

double count_series(size_t N, double (*law_fun)(size_t)){
    // считаем в обратном порядке, чтобы избеать ошибки округления
    // c первого элемента
    double result = 0.;
    for (size_t i = N-1; i>=1; i--){
        result += law_fun(i);
    }
    return result;
}


#define TEST
#ifdef TEST

double f(size_t i){
    return 1./(i*i);
}

int main(void){
    double result;
    // test
    for(size_t i=10; i<500; i+=10 ){
        result = count_series(i, f);
        printf("Numer = %ld , Sum = %lf , Analitical = %lf  \n", i, result, M_PI*M_PI/6);
    }
    return 0;
}

#endif


