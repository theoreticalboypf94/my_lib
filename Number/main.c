#include <stdio.h>

#include "Number.h"

int main() {
    Number n = string_to_Number("-1234.1234e-1324", 20);
    Number_simple_print(&n);
    return 0;
}