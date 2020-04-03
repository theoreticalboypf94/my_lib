//
// Created by alpharius on 02.04.2020.
//

#include "Number.h"
#define AMOUNT nptr->amount_of_signs /* бич макрос */
#define SIGN nptr->data[0]
#define POWER nptr->data[1]
#define NUMBERS &nptr->data[2]  /* указатель на цифры в числе */

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
const char* Number_to_string(Number* nptr){
    /*
     *  1 символ на знак  + 1 символ на точку + 1 символ на e + AMOUNT + 11 символов на показатель степени
     *
     *  метод сделан константным, чтобы не расхуярить указатель на память - а то всему пизда.
     */

    static char*  result; // сделал указатель крепостным этой функции
    // ??? спасет ли это меня от утечки памяти ???
    if (result != NULL){
        free(result);
    }
    result = calloc(1+1+1+AMOUNT+11,sizeof(char));
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
    printf("power == %d ", POWER); // TODO - ошибка происходит где то тут.
    itoa(POWER, tmp_ch_ptr);
    strcat(result, tmp_ch_ptr);

    return result;
}
void Number_simple_print(Number* nptr){
    /*
     * вывод числа без учета ошибочных чисел
     */
    printf("amount of signs = %d , errors signs = %d\n", AMOUNT, ERROR);
    printf("| SIGN | \tPOWER | \t NUMBERS\n");
    printf("| %c | \t %d |", (SIGN == +1? '+' : '-'), POWER);
    for (size_t i=0; i<AMOUNT; i++){
        printf("%d", get_int_from_Number(nptr, i));
    }
    printf(" |\n");
}
Number string_to_Number(const char* chptr, int amount_of_sign){
    /*
     * все возможные формы записи
     * 1.234
     * -1.234
     * 1.234e123
     * -1.234e123
     * 1.234e-123
     * 123.4
     * 123.4e-123
     * -123.4e-123
     * 123
     * -123e123
     * 0.00000123
     * 0.00000123
     *
     * реализация конечного автомата
     * A - ablhabit {'0'..'9','.','e','+','-'}
     * S - states  {S_SIGN, S_LEFT, S_RIGHT, S_POWER}
     *
     *                    '.'
     * S_SIGN -> S_LEFT  -> S_RIGHT
     *              | 'e'     | 'e'
     *              V         V
     *           S_POWER  <----
     * лучше прикреплю картинку к этой хуйне, а то нормально не передать замысел
     *
     * код сделан в предположении, что у меня число более точное чем строковое представление, в противном случае
     * надо переделать программу - добавить счетчик на числа - и обрывать запись (но это потом, если потом)
     *
     */

    enum STATE{S_SIGN, S_LEFT, S_RIGHT, S_POWER};
    enum STATE  state = S_SIGN;
    Number result = new_Number(amount_of_sign);
    Number* nptr = &result;                         // для того, чтобы хуярить макросы
    int *numbers_ptr = NUMBERS;
    int power_modificator = -1;

    while(*chptr++ != '\0'){
        printf("* = %c\n", *chptr);
        switch (state){
            case S_SIGN:
                if (*chptr == '+'){
                    SIGN = 1;
                } else if (*chptr == '-'){
                    SIGN = -1;
                } else {  // если запись началась с числа
                    SIGN = +1;
                    *numbers_ptr = (int) (*chptr - '0');
                    numbers_ptr++;
                    power_modificator++; // один символ до точки ничего не меняет, каждый последующий увеличивает степень
                }
                state = S_LEFT;
                break;
            case S_LEFT:
                if (*chptr >= '0' && *chptr <= '9'){
                    *numbers_ptr = (int) (*chptr - '0');
                    numbers_ptr++;
                } else if (*chptr == 'e'){
                    state = S_POWER;
                } else if (*chptr == '.'){
                    state = S_RIGHT;
                }

                break;
            case S_RIGHT:
                if (*chptr >= '0' && *chptr <= '9'){
                    *numbers_ptr = (int) (*chptr - '0');
                    numbers_ptr++;
                } else if (*chptr == 'e'){
                    state = S_POWER;
                }

                break;
            case S_POWER:
                POWER = atoi(chptr);
                goto out_of_finit_automat;
        }
    }
    POWER += power_modificator;
    out_of_finit_automat:
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