//
// Created by alpharius on 18.08.19.
//

#include "graph_in_bpm.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <float.h>

extern void save_bitmap(const char *file_name, int width, int height, int dpi, struct rgb_data *pixel_data);
//typedef struct rgb_data rgb;
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

void print_dot(rgb* ptr, size_t size, size_t i, size_t j){
    // произвожу инвертирование по индексу - ведь наростание
    // в битовой карте проходит слева на право сверху вниз

    i = size - i;
    j = j - size/2;
    ACCESS(ptr, size, i, j).r = 0;
    ACCESS(ptr, size, i, j).g = 0;
    ACCESS(ptr, size, i, j).b = 0;
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

    X = (double*) malloc(number_of_dots*sizeof(double));
    Y = (double*) malloc(number_of_dots*sizeof(double));

    rewind(fp);
    while(!feof(fp)){
        // записываем и инкрементируем указатель
        // конечный offset = number_of_dots
        fscanf(fp, "%lf %lf", X++, Y++);
        //X++;
        //Y++;
    }
    // возвращение на исходную позицию
    X = X - number_of_dots;
    Y = Y - number_of_dots;

    printf("sizeof rgb = %ld\n", sizeof(rgb));
    rgb *image = (rgb*) malloc(size*size*sizeof(rgb));
    init_graph(image, size);
    // производим маштабирование по наибольшей из размерности
    // это позволит всегда помещать график внутри изображения
    double pixel_scale = (((SUP - INF) > (RL - LL))? (SUP - INF) : (RL - LL)) / size;
    printf("pixel_scale = %lf\n", pixel_scale);
    fclose(fp);

    for(int dot=0; dot<number_of_dots; dot++){
        size_t ind_x = *(X+dot)/pixel_scale;
        size_t ind_y = /*size -*/ *(Y+dot)/pixel_scale;

        print_dot(image, size, ind_y, ind_x);        
    }
    save_bitmap("test.bmp", size, size, 96, image);
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