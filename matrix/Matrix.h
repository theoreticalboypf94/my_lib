//
// Created by alpharius on 27.12.19.
//

#ifndef ALLOC_TEST_MATRIX_H
#define ALLOC_TEST_MATRIX_H

#include <glob.h>

typedef struct matrix (*m_fp_v)(void);
typedef void (*v_fp_v)(void);

typedef struct matrix {
    size_t height, width;
    double *data;
    double (*ACCESS)(struct matrix*, size_t, size_t);
    void  (*PRINT)(struct matrix*);
    void (*WRITE)(struct matrix*, size_t, size_t, double);
    struct matrix (*T)(struct matrix*);
    struct matrix (*MINOR)(struct matrix*, size_t, size_t);
} Matrix;
Matrix new_Matrix(size_t,size_t);


#endif //ALLOC_TEST_MATRIX_H
