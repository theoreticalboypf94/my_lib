#define DEBUG

#include "Matrix.h"
#include "Matrix_functions.h"
#include <stdio.h>



int main(int argc, char** argv){
    Matrix M = new_Matrix(3, 3);
    M.WRITE(&M, 0,0, 1);
    M.WRITE(&M, 1,1, 2);
    M.WRITE(&M, 1,2, 3);
    M.WRITE(&M, 2,2, 4);

    M.PRINT(&M);
    printf("\n");
//    Matrix M2 = M.T(&M);
//    M2.PRINT(&M2);
//    printf("\n");
//    M2 = M2.MINOR(&M2, 0, 2);
//    M2.PRINT(&M2);


    printf("\n %e \n",det(&M));

    return 0;
}