#include "Matrix.h"
#include <stdio.h>



int main(int argc, char** argv){
    Matrix M = new_Matrix(3, 4);
    M.WRITE(&M, 0,0, 1);
    M.WRITE(&M, 1,1, 2);
    M.WRITE(&M, 1,2, 3);
    M.WRITE(&M, 2,2, 4);

    M.PRINT(&M);
    printf("\r");
    Matrix M2 = M.T(&M);
    M2.PRINT(&M2);
    printf("\n");
    M2 = M2.MINOR(&M2, 1, 1);
    M2.PRINT(&M2);


    return 0;
}