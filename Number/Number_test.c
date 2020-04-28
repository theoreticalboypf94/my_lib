//
// Created by alpharius on 27.04.2020.
//
// gcc Number_test.c Number.c -o test && ./test
#include "Number.h"
#define PRECISION 50


bool TEST_ADD_UNIT(const char* first, const char* second, const char* result){
    Number firstN = string_to_Number(first, PRECISION);
    Number secondN = string_to_Number(second, PRECISION);
    Number resultN = string_to_Number(result, PRECISION);
    Number res = ADD(&firstN, &secondN);
    /*
     *  вывод тех хуевин которые не прокнули
     *
     */
    bool result_bool = EQUAL(&res, &resultN);
    if (!result_bool){
        printf("(%s) +", Number_to_string(&firstN));
        printf(" (%s)", Number_to_string(&secondN));
        printf(" != (%s)", Number_to_string(&resultN));
        printf("  ADD = (%s)\n", Number_to_string(&res));
    }
    return result_bool;
}

void TEST_ADD(void){
    int number_of_test = 5;
    int success = 0;
    success += TEST_ADD_UNIT("222", "333", "555");
    success += TEST_ADD_UNIT("-222", "-333", "-555");
    success += TEST_ADD_UNIT("2e2", "3", "203");
    success += TEST_ADD_UNIT("2e2", "3", "2.03e2");
    success += TEST_ADD_UNIT("99", "11.23", "110.23");
    printf("number of test: %d  , correct from them %d \n", number_of_test, success);
}

int main(void){
    // TEST
    TEST_ADD();
}





