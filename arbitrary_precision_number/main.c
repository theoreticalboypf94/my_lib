#include <stdio.h>
#include "number.h"
#include <malloc.h>

int main() {
    Number n = string_to_Number("-123.456e-643", 16);
    Number_simple_print(&n);
    Number_move_right(&n);
    Number_simple_print(&n);


    printf("\n %s", Number_to_string(&n));


    return 0;
}