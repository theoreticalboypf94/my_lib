//
// Created by alpharius on 31.12.19.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "Matrix_functions.h"

#define ISODD(n)    (((n)%2 != 0) ? 1 : 0)
#define SIGN(n)     (ISODD(n) ? -1 : 1);

// просто пара чисел, которая используется как возвращаемое значение
struct _int_couple {
    int first;
    int second;
};

// функция считывает в файле число строк и столбцов
static struct _int_couple _matrix_functions_count_rows_cols(char*);

// ищем определитель матрицы
double det(Matrix* M){
    /*
     *  это первый вариант определителя - неоптимальный
     *  раскладываем по первой строке
     *
     *  оптимизацией тут является просмотр - и разложение по строке-столбцу в котором наибольшее число нулей
     *  либо свершение операций над строками, столбцами.
     */
    double determinant = 0;
    assert(M != NULL);
    assert(M->width == M->height);

    if (M->width == 1){
        return M->ACCESS(M, 0, 0);
    }

    for(size_t j=0; j<M->width; j++){
        Matrix M_0_j = M->MINOR(M, 0, j);
        determinant +=  det(&M_0_j) * M->ACCESS(M, 0, j) * SIGN(j) ;
    }

    return determinant;
}


// произведение матриц. определяется классическим образом
Matrix MAT_MUL(Matrix* m1, Matrix* m2){
    assert(m1->width == m2->height);

    Matrix result = new_Matrix(m1->height, m2->width);
    double conv = 0.;
    for(size_t j=0; j<m1->height; j++){
        for(size_t i=0; i<m2->width; i++){
            conv = 0.;
            for (size_t k=0; k<m1->width; k++){
                conv += m1->ACCESS(m1, i, k) * m2->ACCESS(m2, k, j);
            }
            result.WRITE(&result, i, j, conv);
        }
    }
    return result;
}

// считывание матрицы из текстового файла.
Matrix READ_MAT_FF(char* path_to_file){
    //Use the %lf format specifier to read a double:
    struct _int_couple ic =  _matrix_functions_count_rows_cols(path_to_file);
    printf("\nread input file: \"%s\"\n we have %d rows and  %d columns\n",path_to_file, ic.first, ic.second);
    Matrix result = new_Matrix(ic.first, ic.second);

    FILE *fp = fopen(path_to_file, "r");
    for(size_t i=0; i<ic.first; i++){
        for(size_t j=0; j<ic.second; j++){
            double tmp;
            fscanf(fp, "%lf",&tmp);
            result.WRITE(&result, i,j,tmp);
        }
    }
    return result;
}

// расчитываю количиство строк и столбцов, по наличному файлу
static struct _int_couple _matrix_functions_count_rows_cols(char* path_to_file){
    struct _int_couple result = {1,1};
    FILE *fp;
    fp = fopen(path_to_file, "r");

    if (fp == NULL){
        printf("\nFile not exists or something like that\n");
        printf("file = %s \n line = %d",__FILE__, __LINE__);
        abort();
    }
    char ch;
    bool just_count_column = true;
    while ((ch = fgetc(fp)) != EOF){
        // just count amount of columns from the first line
        if (ch == ' ' && just_count_column){
            result.second++;
        }
        if (ch == '\n'){
            result.first++;
            just_count_column = false;
        }
    }

    fclose(fp);
    return result;
}

// матрица поворота в двухмерном пространстве
Matrix Rotation_matrix_2D(double angle){
    /*
     Матрица поворота в двухмерном пространстве
     это промежуточная функция - в ней я использую стандартную математическую библиотеку
     от чего в дальнейшем я хочу избавиться - написав свои реализации тригинометрических
     функций
    */
    Matrix result = new_Matrix(2,2);
    result.WRITE(&result,0,0,cos(angle));
    result.WRITE(&result,1,1,cos(angle));
    result.WRITE(&result,0,1,-sin(angle));
    result.WRITE(&result,1,0,sin(angle));
    return result;
}

// матрица поворота в трехмерном пространстве
Matrix Rotation_matrix_3D(double alpha, double betta, double gamma){
    Matrix result = new_Matrix(3,3);
    result.WRITE(&result,0,0,cos(alpha)*cos(gamma) - sin(alpha)*cos(betta)*sin(gamma));
    result.WRITE(&result,0,1,-cos(alpha)*sin(gamma)-sin(alpha)*cos(betta)*cos(gamma));
    result.WRITE(&result,0,2,sin(alpha)*sin(betta));
    result.WRITE(&result,1,0,sin(alpha)*cos(gamma)+cos(alpha)*cos(betta)*sin(gamma));
    result.WRITE(&result,1,1,-sin(alpha)*sin(gamma)+cos(alpha)*cos(betta)*cos(gamma));
    result.WRITE(&result,1,2,-cos(alpha)*sin(betta));
    result.WRITE(&result,2,0,sin(betta)*sin(gamma));
    result.WRITE(&result,2,1,sin(betta)*sin(gamma));
    result.WRITE(&result,2,2,cos(betta));
    return result;
}

#undef ISODD
#undef SIGN