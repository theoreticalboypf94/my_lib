#include <stdio.h>
#include "number.h"
#include <malloc.h>
#include <string.h>

int main() {
    Number n1 = string_to_Number("1.111111111111181e2", 16);
    Number n1_other = Number_copy(&n1);
    Number n2 = MULTIPLY(&n1, &n1_other);
    Number_simple_print(&n2);
    printf("%s", Number_to_string(&n2));
    return 0;
}