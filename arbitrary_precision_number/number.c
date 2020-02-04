//
// Created by alpharius on 04.02.2020.
//

#include "number.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

char* _itoa(int, int);

Number new_Number(int n_sign){
    Number result;
    result.amount_of_signs = n_sign;
    result.data = (int*) calloc(n_sign + 2, sizeof(int));
}

Number string_to_Number(const char* str){
    assert(str != NULL);
    Number result = new_Number(strlen(str) + 2);

    // предусмотрены 3 различных случая подстановки числа
    // 3.14 +3.14 -3.14 в последних случаях мы учитываем знак и сдвигаем коретку
    if (*str == '-'){
        result.data[0] = -1;
        str++;
    } else if (*str == '+'){
        result.data[0] = +1;
        str++;
    }
    else result.data[0] = +1;

    int exponenta = 0;
    int signs_before_dot = 0;
    int exponenta_sign = 1;
    bool before_dot = true;
    int *num_ptr = result.data+2;

    // читаем либо до конца либо до e
    while(*str != '\0' || *str != 'e'){
        if (*str == '.'){
            before_dot = false;
            continue;
        }
        before_dot ? signs_before_dot++ : 0;
        *num_ptr = (int) *str - '0'; // превращение char to int
        num_ptr++;
        str++;
    }

    if (*str == 'e'){
        str++;
        *str == '-' ? exponenta_sign = -1, str++ : 0;       // делаю две операции
        *str == '+' ? str++ : 0;                // default exponenta_sign = 1

        while(*str !='\0'){
            exponenta *= 10;
            exponenta += (int) *str - '0';
        }
    }
    exponenta *= exponenta_sign;
    return result;
}

char* Number_to_string(Number* Input){
    //3 для знака +- для точки . и для буквы e
    //10 для успешно записи показателя экспоненты
    char result[Input->amount_of_signs + 3+10];
    char *second;
    if(Input->data[0] == -1){
        result[0] = '-';
    } else {
        result[0] = '+';
    }
    int* num_ptr = &Input->data[2];

    result[1] = *num_ptr;
    num_ptr++;
    result[2] = '.';
    for(size_t i=0; i<Input->amount_of_signs-1; i++){
        result[3+i] = *num_ptr;
        num_ptr++;
    }
    result[Input->amount_of_signs+2] = 'e';
    second = _itoa(Input->data[1], 10);
    return result;
}




char* _itoa(int val, int base){
    static char buf[32] = {0};
    int i = 30;
    for(; val && i ; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i+1];
}