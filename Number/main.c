#include <stdio.h>

#include "Number.h"

int main() {
    Number n = string_to_Number("-1234.9991234666e4", 20);
    Number res, n2 = copy_Number(&n);
    res = SUBSTRACT(&n, &n2);
    Number_simple_print(&res);
    return 0;
}