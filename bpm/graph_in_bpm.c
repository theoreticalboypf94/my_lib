//
// Created by alpharius on 18.08.19.
//

#include "graph_in_bpm.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void draw_graphic(const char *input_file,int size){
    FILE *fp = fopen(input_file, "r");
    double *X = (double*) malloc(sizeof(double));
    double *Y= (double*) malloc(sizeof(double));;
    double LL, RL, SUP, INF; // left limit, right.., function supremum, infinum

    while(feof(fp)){
        fscanf(fp,"%lf %lf", X, Y);
        printf("%lf %lf \n", *X, *Y);
    }

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