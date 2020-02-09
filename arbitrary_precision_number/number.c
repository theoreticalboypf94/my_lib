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

#define POWER_SYMBOLS 11
#define E_SYMBOL 1
#define SIGN_SYMBOL 1
#define DOT_SYMBOL 1

static void itoa(int, char*);
static void reverse(char* );

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
    //warning: pointer/integer type mismatch in conditional expression ликвидировать безобразие
    *ch_ptr == '-' ? *(i_ptr - 2)=-1, ch_ptr++, 1 : (*(i_ptr - 2) = +1);
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

    *ch_ptr == '\0' ? (result.data[1] = 0), NULL : NULL;    // дана возможность не прибегать к научной нотации e.g 12.12 not 12.12e0
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
    exp_value += (signs_before_dot - 1);
    result.data[1] = exp_value;

    // принуждение к научной нотации - с ненулевым первым знаком
    while(result.data[2] == 0){
        Number_move_left(&result);
    }

    return result;
}

char* Number_to_string(Number* input){
    /*
     *  вначале проводим почисленное считывание разрядов а затем отбрасываем ненужные нули
     *  в младших разрядах если они есть, так достигается более наглядное представление числа.
     */
    // перевод к научной нотации - с ненулевым значащим числом
    while(input->data[2] == 0){
        Number_move_left(input);
    }

    // показатель экспоненты
    char *power = (char*) calloc(POWER_SYMBOLS , sizeof(char)); // 10 символов на 10 знаков и 1 на знак
    itoa(input->data[1],power); // степень
    // знак числа
    char *sign = (char*) calloc(1, sizeof(char)); *sign = input->data[0] > 0 ? '+' : '-';
    //числа по разряду
    char *numbers = (char*) calloc(input->amount_of_signs+DOT_SYMBOL, sizeof(char));
    for(size_t i=0; i<input->amount_of_signs+1; i++){
        numbers[i] = i < 2 ? (i == 1? '.' : input->data[i+2] + '0') :input->data[i+2-1] + '0';
    }
    // оптимизация по разрядам
    for(int i=input->amount_of_signs+DOT_SYMBOL-1; i>1; i--){
        printf("*");
        if (numbers[i] == '0'){
            numbers[i] = 0;
        } else {

            break;
        }
    }

    char *result = (char*) calloc( (POWER_SYMBOLS+E_SYMBOL+SIGN_SYMBOL+DOT_SYMBOL+input->amount_of_signs),
            sizeof(char));

    // запись в результат - с освобождением алоцированной памяти
    strcat(result, sign);
    free(sign);
    strcat(result, numbers);
    free(numbers);
    strcat(result,"e");
    strcat(result, power);
    free(power);

    return result;
}

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
    for(size_t i=aos-1; i>2; i--){
        n->data[i] = n->data[i-1];
    }
    n->data[2] = 0;
    n->data[1]++;
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
    n->data[1]--;
}

void Number_move_left_pos(Number *n, int pos){
    for(size_t i=0; i<pos; i++){
        Number_move_right(n);
    }
}

Number ADD(Number* first, Number* second){
    // проверка на равную точность
    assert(first->amount_of_signs == second->amount_of_signs);

    // выравниваем порядки
#define o1 first->data[1]
#define o2 second->data[1]
    if(o1 != o2){
        if (o1 < o2){
            Number_move_right_pos(first, o2-o1);
        } else {
            Number_move_right_pos(second,o1-o2);
        }
    }
#undef o1
#undef o2
    Number_simple_print(first);
    Number_simple_print(second);
    Number result = new_Number(first->amount_of_signs);

    return result;
}

bool EQUAL(Number* first, Number* second){
    bool result = true;
    result = result && first->amount_of_signs == second->amount_of_signs;
    result = result && first->data[0] == second->data[0] && first->data[1] == second->data[1];
    if (!result) return result;

    // проверка с точностью до последнего числа в "разряде" - выполняет роль помойки
    for(size_t i=0; i<first->amount_of_signs-2; i++){
        result = result && first->data[2+i] == second->data[2+i];
        if (!result) return result;
    }
    return result;
}



// Спиздил у Ричи и Кернигана
static void itoa(int n, char *s)
{
    int i, sign;

    if ((sign = n) < 0)  /* записываем знак */
        n = -n;          /* делаем n положительным числом */
    i = 0;
    do {       /* генерируем цифры в обратном порядке */
        s[i++] = n % 10 + '0';   /* берем следующую цифру */
    } while ((n /= 10) > 0);     /* удаляем */
    if (sign < 0)
        s[i++] = '-';
    else s[i++] = '+';  // отсебятинка
    s[i] = '\0';
    reverse(s);
}

static void reverse(char* s)
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}