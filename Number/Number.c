//
// Created by alpharius on 02.04.2020.
//

#include "Number.h"
#define AMOUNT nptr->amount_of_signs /* бич макрос */
#define SIGN nptr->data[0]
#define POWER nptr->data[1]
#define NUMBERS &nptr->data[2]  /* указатель на цифры в числе */
#define ABS(x) (((x) >= 0) ? x : -x)  /* вроде не должно хуярить */
#define SIGN_OF_INT(x)  ((x) > 0) ? +1 : -1

/*
 * декларация методов-утилит, полезных внутри других функций
 */
int _number_not_zero_elements(Number *nptr);
static void _account_of_signs_in_numbers(Number*, Number*);
static void _change_sign(Number*);
static void position_addition(Number*, Number*);
static void position_alignment(Number*);


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
    result.data = calloc(SAP + amount_of_signs + NUMBER_ERROR, sizeof(int));
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
    for(size_t i=1; i< nptr->amount_of_signs + NUMBER_ERROR; i++){
        nptr->data[SAP + i - 1] = nptr->data[SAP + i];
    }
    nptr->data[SAP + AMOUNT + NUMBER_ERROR - 1] = 0;
}
void move_right(Number* nptr){
    /*
     * перенос данных влево - по сути умножение числа на 10
     * 12.3e+4 => 1.23e+5
     */
    assert(nptr != NULL);
    assert(nptr->data[2] == 0); // защита от потерии информации
    nptr->data[1]++;
    for(size_t i= AMOUNT + NUMBER_ERROR - 1; i > 1; i++){
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
    assert(index >= -2);
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
    printf("amount of signs = %d , errors signs = %d\n", AMOUNT, NUMBER_ERROR);
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
     * NB - я предполагаю контроль длины вводимого текстового сообщения со стороны пользователя функции
     * иначе беда sigfault или что то такое
     *
     */

    enum STATE{S_SIGN, S_LEFT, S_RIGHT, S_POWER};
    enum STATE  state = S_SIGN;
    Number result = new_Number(amount_of_sign);
    Number* nptr = &result;                         // для того, чтобы хуярить макросы
    int *numbers_ptr = NUMBERS;
    int power_modificator = -1;

    while(*chptr != '\0'){
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
        chptr++;
    }
    POWER += power_modificator;
    out_of_finit_automat:
    return result;
}

// копирование и размножение чисел
void MOV(Number* left, Number* right){
    assert(left->amount_of_signs == right->amount_of_signs);
    for(size_t i=0; i< SAP + left->amount_of_signs + NUMBER_ERROR; i++){
        left->data[i] = right->data[i];
    }
}
Number copy_Number(Number* _copy){
    Number result = new_Number(_copy->amount_of_signs);
    for(size_t i=0; i< SAP + _copy->amount_of_signs + NUMBER_ERROR; i++){
        result.data[i] = _copy->data[i];
    }
    return result;
}

// арифметический блок
Number ADD(Number* first, Number* second){
    /*
     *  сложение левого и правого элемента - используем конечный автомат.
     *  1.5e6
     *  2.4e3 => 0.24e4 => 0.024e5 => 0.0024e6 - меньшую степень мы приводим к большей, чтобы не терять знаки
     *  меньшую степень мы сдвигаем вправо
     *
     *  использованы функциии:
     *      void _change_sign(Number* input)
     *      static void position_addition(Number* first, Number* second)
     *      static void position_alignment(Number* nptr)
     */
    assert(first->amount_of_signs == second->amount_of_signs);
    Number result = new_Number(first->amount_of_signs);
    // приравниваем показатели степени
    while(first->data[1] != second->data[1])
        (first->data[1] < second->data[1]) ? move_right(first) : move_right(second);
    _account_of_signs_in_numbers(first, second);
    // в result - занесена поразрядовая сумма
    // возможно с перегрузками по разрядам, которую мы должны ликвидировать
    position_addition(&result, first);
    position_addition(&result, second);
    position_alignment(&result);

    return result;
}

Number SUBSTRACT(Number* first, Number* second){
    /*
     * result = first - second
     */
    second->data[0] *= -1;
    Number result = ADD(first, second);
    second->data[0] *= -1; // возвращаем знак к исходному
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

void _account_of_signs_in_numbers(Number* n1, Number* n2){
    /*
     *  если sign == '-' -1 тогда мы все числа в разряде заменяем на отрицательные
     *  знак будет проставлен 0
     */
    if(get_int_from_Number(n1, -2) == -1){
        _change_sign(n1);
    }
    if(get_int_from_Number(n2, -2) == -1){
        _change_sign(n2);
    }
}


// BEGIN ADD_UTILS
static void _change_sign(Number* input){
    /*
     * все знаки в числах - поменяли знак на отрицательный - это нужно для проведения операции сложения
     * на будущее вводится 0й знак - это не -1 и не +1 а новая категория означающая необходимость
     * вычислить знак после выполнения всех операций.
     *
     * для функции _account_of_signs_in_numbers
     */
    for(size_t i=0; i< input->amount_of_signs + NUMBER_ERROR; i++){
        input->data[SAP + i] *= -1;
    }
    input->data[0] = 0;
}

static void position_addition(Number* first, Number* second){
    /*
     * поразрядовое сложение чисел, даже с учетом перегрузки разряда
     */
    for(size_t i=0; i< first->amount_of_signs + NUMBER_ERROR; i++){
        first->data[SAP + i] += second->data[SAP + i];
    }
}

static void position_alignment(Number* nptr){
    /*
     * выравнивание разрядов - ликвидация перегрузки разряда
     * в разряде обязано быть число 0..9
     * если у нас больше - то добавляем к старшему разряду кроме последнего, либо вычитаем
     * +A -B +C -D
     */
    for(size_t i=SAP+nptr->amount_of_signs+NUMBER_ERROR-1; i>SAP; i--){
        int position = nptr->data[i];
        if(ABS(position) >= 10){
            nptr->data[i] = position % 10;
            if (position > 0){
                nptr->data[i-1] += position / 10;
            } else {
                nptr->data[i-1] -= position / 10;
            }
        }
    }

    // ликвидация перегрузки старшего разряда - возможно стоит объединить
    if(ABS(nptr->data[SAP]) >= 10){
        move_right(nptr);
        nptr->data[SAP] = nptr->data[SAP+1] / 10;
        nptr->data[SAP+1] %= 10;
    }

    // ликвидация нулевого старшего разряда
    int counter = 0;    // защита от получения бесконечного цикла из за нулевого результата суммирования TODO вчесть из числа число
    while (nptr->data[SAP] == 0 && counter++ < nptr->amount_of_signs){
        move_left(nptr);
    }
    if (counter == nptr->amount_of_signs) goto finish;  // оптимизационный прыжек

    // теперь мы гарантируем то что старший разряд отличен от нуля - нотация сохранена.

    // приведение к однозначному выражению каждый разряд либо только + либо только -
    // прибавление(вычитание) подстраивается под знак старшего разряда
    for(size_t i=SAP + nptr->amount_of_signs + NUMBER_ERROR; i>SAP; i--){
        // TODO - потом исправить на оптимальный код - без дубляжа
        if (nptr->data[SAP]>0){ // ровняем разряды под + , прибавляем 10 и вычитаем из старшего разряда 1
            if(nptr->data[i] < 0){
                nptr->data[i] += 10;
                nptr->data[i-1] -= 1;
            }
        } else {
            if (nptr->data[i] > 0){
                nptr->data[i] -= 10;
                nptr->data[i-1] += 1;
            }
        }
// TODO - проверить эту оптимизацию
//        if (nptr->data[SAP] * nptr->data[i] < 0){
//            nptr->data[i] += 10 * SIGN_OF_INT(nptr->data[SAP]);
//            nptr->data[i-1] -= 1 * SIGN_OF_INT(nptr->data[SAP]);
//        }
    }

    // повторная ликвидация нулевого старшего разряда
    counter = 0;    // защита от получения бесконечного цикла из за нулевого результата суммирования
    while (nptr->data[SAP] == 0 && counter++ < nptr->amount_of_signs){
        move_left(nptr);
    }


    finish:;
}
// END ADD_UTILS


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