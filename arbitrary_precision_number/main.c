#include <stdio.h>
#include "number.h"
#include <malloc.h>
#include <string.h>

int main() {
    Number n1 = string_to_Number("-0.00456e+12", 16);
    Number n2 = string_to_Number("-0.00456e+10", 16);


    ADD(&n1, &n2);


    return 0;
}