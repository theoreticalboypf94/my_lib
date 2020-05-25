#include <stdio.h>
#include "Linked_list.h"

int main() {
    LinkedList *LL = new_LL(STRING, STRING);
    push_LL(LL, 4, "asdf", "rrrrr", "12345", "USSE");
    print_LL(LL);
    printf(" bool = %d",in_LL(LL, "USS2"));
    return 0;
}