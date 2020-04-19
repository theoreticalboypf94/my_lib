#include <stdio.h>

#include "Number.h"

void __ZATRAVKA(Number*, Number*);

int main() {
    Number res, a, b;
    a = string_to_Number("22", 20);
    b = string_to_Number("2.1234", 20);
    res = new_Number(20);
    Number_simple_print(&a);
    Number_simple_print(&b);
    DEVIDE_ptr(&res, &a, &b);
    Number_simple_print(&res);
    return 0;
}