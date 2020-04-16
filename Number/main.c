#include <stdio.h>

#include "Number.h"

int main() {
    Number res, a, b;
    a = string_to_Number("22", 20);
    b = string_to_Number("44e2", 20);
    Number_simple_print(&a);
    Number_simple_print(&b);
    res = DEVIDE(&a, &b);
    Number_simple_print(&res);
    return 0;
}