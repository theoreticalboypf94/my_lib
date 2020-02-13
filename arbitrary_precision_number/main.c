#include <stdio.h>
#include "number.h"
#include <malloc.h>
#include <string.h>

int main() {
    Number n1 = string_to_Number("1.5e2", 16);
    Number n2 = string_to_Number("-1.5234e-2", 16);
    Number_simple_print(&n1);
    Number_simple_print(&n2);

//    printf("\n %d \n", LE(&n1, &n2));


    Number n3 = DEVIDE(&n1, &n2);
    Number_simple_print(&n3);
    printf("\nn1+n2 = %s", Number_to_string(&n3));



    return 0;
}