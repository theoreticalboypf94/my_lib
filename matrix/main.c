

#include "Matrix.h"
#include "Matrix_functions.h"
#include <stdio.h>
#include <math.h>



int main(int argc, char** argv){

    Matrix M1 = READ_MAT_FF("/home/alpharius/Git/my_lib/matrix/mat.txt");
    M1.PRINT(&M1);

    printf("%f \n",det(&M1) );

//    M1.ROW_OP(&M1, 0, 1, 1);
//    M1.ROW_OP(&M1, 1, 0, -1);
//    M1.ROW_OP(&M1, 0, 1, 1);
//    M1.ROW_OP(&M1, 0, 0, -2);

    M1.SWAP(&M1, 0, 1);
    M1.PRINT(&M1);
    printf("%f \n",det(&M1) );

    return 0;
}