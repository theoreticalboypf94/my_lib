//
// Created by alpharius on 31.12.19.
//

#ifndef ALLOC_TEST_MATRIX_FUNCTIONS_H
#define ALLOC_TEST_MATRIX_FUNCTIONS_H

#include "Matrix.h"
//#define DEBUG

typedef double angle;

double det(Matrix* M);                          // нахождение определителя матрицы - классическим способом
Matrix MAT_MUL(Matrix*, Matrix*);               // матричное произведение
Matrix READ_MAT_FF(char*);                      // прочитать матрицу из текстового файла read matrix from file акроним
Matrix rotation_matrix_2D (angle);              // конкретная матрица поворота для данного угла, arg in pi
Matrix Rotation_matrix_3D(angle,angle,angle);   // конкретная матрица поворота для данного угла, arg in pi

#endif //ALLOC_TEST_MATRIX_FUNCTIONS_H
