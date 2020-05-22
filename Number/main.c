#include <stdio.h>

#include "Number.h"
#include "Number_test.c"
#include "gtest/gtest.h"

void __ZATRAVKA(Number*, Number*);
int _number_not_zero_elements(Number*);

TEST(GreaterTest,AisGreater){
    EXPECT_EQ(0,UNITED_TEST_OF_Number());
};


int main() {
    //Number res, a, b;
    //a = string_to_Number("3", 20);
    //Number_simple_print(&a);
    //printf("%s \n", Number_to_string(&a));
    UNITED_TEST_OF_Number();
    //int_to_Number(&a, 666);

    return 0;
}