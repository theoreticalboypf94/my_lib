//
// Created by alpharius on 04.02.2020.
//

#include "number.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

char* _itoa(int, int);

Number new_Number(int n_sign){
    Number result;
    result.amount_of_signs = n_sign;
    result.data = (int*) calloc(n_sign + 2, sizeof(int));
    return result;
}

Number string_to_Number(const char* str, int precision){
    printf("proof");
    assert(str != NULL);
    Number result = new_Number(precision);

    const char* ch_ptr;
    int* i_ptr;
    ch_ptr = str;
    i_ptr = &result.data[2];

    /* знаки перед точкой */
    *ch_ptr == '-' ? *(i_ptr - 2)=-1, ch_ptr++ : (*(i_ptr - 2) = +1);
    *ch_ptr == '+' ? ch_ptr++ : NULL;

    int signs_before_dot = 0;
    while(*ch_ptr != '\0' && *ch_ptr != '.'){
        signs_before_dot++;
        *i_ptr = (int) *ch_ptr - '0';
        i_ptr++; ch_ptr++;
    }
    *ch_ptr !='\0'? ch_ptr++ : NULL;

    /* знаки после точки ,но перед экспонентой*/
    while (*ch_ptr != '\0' && *ch_ptr != 'e'){
            *i_ptr = (int) *ch_ptr - '0';
            i_ptr++; ch_ptr++;
    }

    *ch_ptr == '\0' ? (result.data[1] = 0) : NULL;    // дана возможность не прибегать к научной нотации e.g 12.12 not 12.12e0
    *ch_ptr == 'e' ? ch_ptr++ : NULL;
    int exp_sign = *ch_ptr == '-' ? ch_ptr++, -1 : +1;
    *ch_ptr == '+' ? ch_ptr++ : NULL;                   // ...e+123
    int exp_value = 0;
    if (*ch_ptr != '\0'){
        exp_value = atoi(ch_ptr);
    }
    exp_value *= exp_sign;

    /*
     *  перевод в научную натацию A.BCDEFeA'B'C' - до . остается только одно число
     *  мы меняем показатель степени.
     */
    exp_value -= (signs_before_dot - 1);
    result.data[1] = exp_value;

    return result;
}

char* Number_to_string(Number* input){
    char *power = (char*) malloc(10 * sizeof(char));
    itoa(input->data[1],power,10); // степень
    //char
    return power;
}

//char* _itoa(int val, int base){
//    static char buf[32] = {0};
//    int i = 30;
//    for(; val && i ; --i, val /= base)
//        buf[i] = "0123456789abcdef"[val % base];
//    return &buf[i+1];
//}

void Number_simple_print(Number *n){
    printf("amount os numbers %d \n", n->amount_of_signs);
    int *ptr = n->data;
    printf(" %d | ", *ptr);
    ptr++;
    printf("%d | ", *ptr);
    ptr++;
    for(size_t i=0; i<n->amount_of_signs; i++){
        printf("%d", *ptr);
        ptr++;
    }
    printf("\n");
}

void Number_move_right(Number* n){
    int aos = n->amount_of_signs + 2;
    for(size_t i=aos-1; i>3; i--){
        n->data[i] = n->data[i-1];
    }
    n->data[2] = 0;
    n->data[1]--;
}

void Number_move_right_pos(Number *n, int pos){
    for(size_t i=0; i<pos; i++){
        Number_move_right(n);
    }
}

void Number_move_left(Number *n){
    assert(n->data[2] == 0); // проверка, чтобы избежать потерии старшего разряда
    int aos = n->amount_of_signs + 2;
    for(size_t i=2; i<aos; i++){
        n->data[i] = n->data[i+1];
    }
    n->data[aos-1] = 0;
    n->data[1]++;
}

void Number_move_left_pos(Number *n, int pos){
    for(size_t i=0; i<pos; i++){
        Number_move_right(n);
    }
}