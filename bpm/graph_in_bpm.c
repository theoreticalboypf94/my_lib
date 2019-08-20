//
// Created by alpharius on 18.08.19.
//

#include "graph_in_bpm.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <float.h>

void draw_graphic(const char *input_file,int size){
    FILE *fp = fopen(input_file, "r");
    double *X = (double*) malloc(sizeof(double));
    double *Y = (double*) malloc(sizeof(double));;
    double LL, RL, SUP, INF; // left limit, right.., function supremum, infinum
    RL = SUP = - DBL_MAX;
    LL = INF = DBL_MAX;
    size_t number_of_dots = 0;

    /*
     * холостое считываение данных - на перед мы не знаем пара
     */
    while(!feof(fp)){
        fscanf(fp,"%lf %lf\n", X, Y);
        printf("%lf %lf \n", *X, *Y);

        if (*X > RL) {RL = *X;}
        if (*X < LL) {LL = *X;}
        if (*Y > SUP) {SUP = *Y;}
        if (*Y < INF) {INF = *Y;}
        number_of_dots++;
    }
    free(X);
    free(Y);

    X = (double*) malloc(number_of_dots);
    Y = (double*) malloc(number_of_dots);

}




//void draw_line()


#define TEST
#ifdef TEST

int main(void){
    printf("we are in main function");
    draw_graphic("test.txt", 10);
    return 0;
}

#endif // TEST