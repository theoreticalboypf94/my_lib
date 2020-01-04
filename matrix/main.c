

#include "Matrix.h"
#include "Matrix_functions.h"
#include <stdio.h>



int main(int argc, char** argv){
    Matrix M = new_Matrix(2, 2);
    M.WRITE(&M, 0, 0, 5);
    M.WRITE(&M, 1, 1, 10);
    M.WRITE(&M, 0, 1, 3);
    M.WRITE(&M, 1, 0, 3);

    M.PRINT(&M);

    Matrix M_I = M.INVERSE(&M);

    M_I.PRINT(&M_I);

    Matrix R = MAT_MUL(&M, &M_I);
    R.PRINT(&R);
    R = MAT_MUL(&M_I, &M);
    R.PRINT(&R);
    R = M;
    R.PRINT(&R);

    printf("rov dev check \n");
    M.ROW_OP(&M, 0, 1, -3./5);

    M.PRINT(&M);

    printf("\n %f \n",det(&M));

    return 0;
}