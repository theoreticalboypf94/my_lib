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

// TODO - возможно стоит загнать это в функцию-макрос уж больно много дубляжа
bool TEST_MUL_UNIT(const char* first, const char* second, const char* result){
    Number firstN = string_to_Number(first, PRECISION);
    Number secondN = string_to_Number(second, PRECISION);
    Number resultN = string_to_Number(result, PRECISION);
    Number res = MUL(&firstN, &secondN);

    /*
     *  вывод тех хуевин которые не прокнули
     */
    bool result_bool = EQUAL(&res, &resultN);
    if (!result_bool){
        printf("(%s) *", Number_to_string(&firstN));
        printf(" (%s)", Number_to_string(&secondN));
        printf(" != (%s)", Number_to_string(&resultN));
        printf("  MUL = (%s)\n", Number_to_string(&res));
    }
    return result_bool;
}

bool TEST_DEVID_UNIT(const char* first, const char* second, const char* result){
    Number firstN = string_to_Number(first, PRECISION);
    Number secondN = string_to_Number(second, PRECISION);
    Number resultN = string_to_Number(result, PRECISION);
    Number res = DEVIDE(&firstN, &secondN);

    /*
     *  вывод тех хуевин которые не прокнули
     */
    bool result_bool = EQUAL(&res, &resultN);
    if (!result_bool){
        printf("(%s) /", Number_to_string(&firstN));
        printf(" (%s)", Number_to_string(&secondN));
        printf(" != (%s)", Number_to_string(&resultN));
        printf("  DEVIDE = (%s)\n", Number_to_string(&res));
    }
    return result_bool;
}

void TEST_ADD(void){
    int number_of_test = 7;
    int success = 0;

    printf("\n ADD_TEST: \n");
    success += TEST_ADD_UNIT("222", "333", "555");
    success += TEST_ADD_UNIT("-222", "-333", "-555");
    success += TEST_ADD_UNIT("2e2", "3", "203");
    success += TEST_ADD_UNIT("2e2", "3", "2.03e2");
    success += TEST_ADD_UNIT("99", "11.23", "110.23");
    success += TEST_ADD_UNIT("-66", "18.23", "-47.77");
    success += TEST_ADD_UNIT("100e4", "1.23", "1000001.23");
    printf("number of test: %d  , correct from them %d \n", number_of_test, success);
}

void TEST_MUL(void){
    int number_of_test = 6;
    int success = 0;

    printf("\n MUL_TEST: \n");
    success += TEST_MUL_UNIT("222", "333", "73926");
    success += TEST_MUL_UNIT("222e-2", "333e-2", "73926e-4");
    success += TEST_MUL_UNIT("222e-2", "333e+2", "73926e0");
    success += TEST_MUL_UNIT("-1.234", "44.34", "-54.71556");
    success += TEST_MUL_UNIT("-1.234", "-44.34", "54.71556");
    success += TEST_MUL_UNIT("-1.234", "0.81037277147", "-0.99999999999398");

    printf("number of test: %d  , correct from them %d \n", number_of_test, success);
}

void TEST_DEVIDE(void){
    int number_of_test = 2;
    int success = 0;
    // обнаружил, что string_to_number - переваривает только научную нотацию
    printf("\n DEVIDE_TEST: \n");
    success += TEST_DEVID_UNIT("222", "333", "0.66666666666666666666666666666666666666666666666666");
    success += TEST_DEVID_UNIT("222", "222", "1");
//    success += TEST_DEVID_UNIT("222e-2", "333e-2", "73926e-4");
//    success += TEST_DEVID_UNIT("222e-2", "333e+2", "73926e0");
//    success += TEST_DEVID_UNIT("-1.234", "44.34", "-54.71556");
//    success += TEST_DEVID_UNIT("-1.234", "-44.34", "54.71556");
//    success += TEST_DEVID_UNIT("-1.234", "0.81037277147", "-0.99999999999398");

    printf("number of test: %d  , correct from them %d \n", number_of_test, success);
}



void UNITED_TEST_OF_Number(void){
    TEST_ADD();
    TEST_MUL();
    TEST_DEVIDE();
}




