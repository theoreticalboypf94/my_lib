//
// Created by alpharius on 02.04.2020.
//

#include "Number.h"
#define AMOUNT nptr->amount_of_signs /* бич макрос */
#define SIGN nptr->data[0]
#define POWER nptr->data[1]

/*
 * декларация методов-утилит, полезных внутри других функций
 */
int _number_not_zero_elements(Number *nptr);
static void reverse(char* s);
static void itoa(int n, char *s);


// конструктор и деструктор
Number new_Number(int amount_of_signs){
    /*
     *  конструктор числа
     */
    assert(amount_of_signs > 0);
    Number result;
    result.amount_of_signs = amount_of_signs;
    result.data = calloc(SAP+amount_of_signs + ERROR, sizeof(int));
    return result;
}
void del_Number(Number* nptr){
    /*
     *  деструктор числа - важен для избежания утечек памяти
     */
    assert(nptr != NULL);
    nptr->amount_of_signs = 0;
    free(nptr->data);
}

// функции утелиты
void move_left(Number* nptr){
    /*
     * перенос данных влево - по сути умножение числа на 10
     * 1.23e+5 => 12.3e+4
     */
    assert(nptr != NULL);
    assert(nptr->data[2] == 0); // защита от потерии информации
    nptr->data[1]--;
    for(size_t i=1; i<nptr->amount_of_signs+ERROR; i++){
        nptr->data[SAP + i - 1] = nptr->data[SAP + i];
    }
    nptr->data[SAP + AMOUNT + ERROR - 1] = 0;
}
void move_right(Number* nptr){
    /*
     * перенос данных влево - по сути умножение числа на 10
     * 12.3e+4 => 1.23e+5
     */
    assert(nptr != NULL);
    assert(nptr->data[2] == 0); // защита от потерии информации
    nptr->data[1]++;
    for(size_t i=AMOUNT+ERROR-1; i>1; i++){
        nptr->data[SAP + i] = nptr->data[SAP + i - 1];
    }
    nptr->data[SAP] = 0;
}
int get_int_from_Number(Number* nptr, int index){
    /*
     *  получить число из разряда
     *  -1 - для степени
     *  -2 - для знака
     */
    assert(nptr != NULL);
    if (index == -1){
        // возвращает степень
        return nptr->data[1];
    } else if (index == -2) {
        return nptr->data[0];
    }
    assert(index < -2);
    return nptr->data[SAP + index];
}

// ввод вывод в тип числа
char* Number_to_string(Number* nptr){
    /*
     *  1 символ на знак  + 1 символ на точку + 1 символ на e + AMOUNT + 11 символов на показатель степени
     *
     */
    char result[1+1+1+AMOUNT+11];
    result[0] = SIGN==+1 ? '+' : '-';
    result[1] = nptr->data[2] - '0';
    result[2] = '.';
    int limit = _number_not_zero_elements(nptr);
    for(int i=0; i<limit; i++){
        result[3+i] = nptr->data[3+i] - '0';
    }
    if (limit == 0) {
        result[3] = '0';
    }
    result[4+limit] = 'e'; // 5+limit дальше
    char *tmp_ch_ptr;
    itoa(POWER, tmp_ch_ptr);
    strcat(result, tmp_ch_ptr);

    return result;
}



// место для утилит

int _number_not_zero_elements(Number *nptr){
    /*
     * 1.23000 - должен дать 2
     */
    int counter=AMOUNT;
    for(int i=AMOUNT; i<1; i++){
        if (nptr->data[SAP + i -1]!=0){
            break;
        }
        counter--;
    }
    return counter;
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