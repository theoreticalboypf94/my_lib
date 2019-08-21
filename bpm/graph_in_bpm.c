//
// Created by alpharius on 18.08.19.
//

#include "graph_in_bpm.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <float.h>

extern void save_bitmap(const char *file_name, int width, int height, int dpi, struct rgb_data *pixel_data);
typedef struct rgb_data rgb;

#define ACCESS(ptr,size,i,j) (*(ptr + j + size*i))

void init_graph(rgb* ptr, size_t size){
    for(size_t i=0; i<size; i++){
        for(size_t j=0; j<size; j++){
            if (j == size/2 || i == size/2){
                ACCESS(ptr, size, i, j).r = 0;
                ACCESS(ptr, size, i, j).g = 0;
                ACCESS(ptr, size, i, j).b = 0;
                continue;
            }


            ACCESS(ptr, size, i, j).r = 255;
            ACCESS(ptr, size, i, j).g = 255;
            ACCESS(ptr, size, i, j).b = 255;
        }
    }
}






void draw_graphic(const char *input_file, int size){
    FILE *fp = fopen(input_file, "r");
    double *X = (double*) malloc(sizeof(double));
    double *Y = (double*) malloc(sizeof(double));;
    double LL, RL, SUP, INF; // left limit, right.., function supremum, infinum
    RL = SUP = - DBL_MAX;
    LL = INF = DBL_MAX;
    size_t number_of_dots = 0;

    /*
     * холостое считываение данных - на перед мы не знаем параметров визуализи
     * руемой функции
     */
    while(!feof(fp)){
        fscanf(fp,"%lf %lf\n", X, Y);
        //printf("%lf %lf \n", *X, *Y);

        if (*X > RL) {RL = *X;}
        if (*X < LL) {LL = *X;}
        if (*Y > SUP) {SUP = *Y;}
        if (*Y < INF) {INF = *Y;}
        number_of_dots++;
    }
    printf("number_of_dots = %ld\n", number_of_dots);
    free(X);
    free(Y);

    X = (double*) malloc(number_of_dots);
    Y = (double*) malloc(number_of_dots);

    rewind(fp);
    while(!feof(fp)){
        // записываем и инкрементируем указатель
        // конечный offset = number_of_dots
        fscanf(fp, "%lf %lf", X, Y);
        X++;
        Y++;
    }
    X = X - number_of_dots;
    Y = Y - number_of_dots;
    printf("sizeof rgb = %ld\n", sizeof(rgb));
    rgb *image = (rgb*) malloc(size*size*12);
//    init_graph(image, size);
    // производим маштабирование по наибольшей из размерности
    // это позволит всегда помещать график внутри изображения
    double pixel_scale = (((SUP - INF) > (RL - LL))? (SUP - INF) : (RL - LL)) / size;
    printf("pixel_scale = %lf\n", pixel_scale);
//    X = X - number_of_dots;
//    Y = Y - number_of_dots;
//    for(size_t dots=0; dots<number_of_dots; dots++){
//        size_t index_x = (int) *(X+dots)/pixel_scale;
//        size_t index_y = (int) size - *(Y+dots)/pixel_scale;
//        //printf("%lf %d\n", *(X+dots), index_y);
//        //BLACK(image, size, index_x, index_y);
//        (ACCESS(image, size, index_x, index_y)).r = 0;
//        (ACCESS(image, size, index_x, index_y)).g = 0;
//        (ACCESS(image, size, index_x, index_y)).b = 0;
//    }
//
//    save_bitmap("test.bmp", size, size, 1000, image);
    fclose(fp);
}




//void draw_line()


#define TEST
#ifdef TEST
#include <math.h>

int main(void){
    printf("we are in main function");

    FILE *fp = fopen("test.txt", "w");
    double x = 0, dx = M_PI/100;
    double y;
    for(int i=0; i<100; i++){
        y = sin(x);
        x+= dx;
        fprintf(fp, "%lf %lf\n", x, y);
    }
    fclose(fp);


    draw_graphic("test.txt", 600);
    return 0;
}

#endif // TEST