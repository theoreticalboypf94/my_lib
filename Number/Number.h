//
// Created by alpharius on 02.04.2020.
//

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#ifndef NUMBER_NUMBER_H
#define NUMBER_NUMBER_H
/*
 *          Класс Чисел
 *
 * число представляется в виде массива в котором 0й элемент отвечает за знак
 *                                               1й элемент отвечает за показатель степени в пределах int
 *                                               2й и последующие отвечают за поразрядовое представление числа
 *
 * sign power 0.......amount_of_signs         NUMBER_ERROR
 * [0]  [1]   [2]     [amount_of_signs+1]     [amount_of_signs+1+NUMBER_ERROR]
 * всего длина массива 2 + amount_of_signs + NUMBER_ERROR
 * SAP + AMOUNT + NUMBER_ERROR - 1 - индекс последнего элемента нашей памяти
 * SAP......SAP + AMOUNT -1  - индекс последнего элемента в значащих цифрах
 *
 */

/* интерфейс класса */
#define NUMBER_ERROR 5 /* разряды выделенные под ошибки */
#define SIGN_AND_POWER 2
#define SAP SIGN_AND_POWER

typedef struct t_Number {
    int amount_of_signs;
    int *data;
} Number;
typedef Number t_N;

/* конструктор и деструктор */
Number new_Number(int amount_of_signs); //..............................................................................+
void del_Number(Number*);   //..........................................................................................+

/* функции утилиты */
void move_left(Number*); // передвинуть числа на разряд влево...........................................................+
void move_right(Number*); // передвинуть числа на разряд вправо.........................................................+
int get_int_from_Number(Number*, int); // получаем число из нужного разряда.............................................+
bool check_correct_number_order(Number*); // проверка корректности разрядового записи числа....TODO.....................-

/* ввод вывод в тип числа */
const char* Number_to_string(Number*); // распечатать число (в научной нотации)...............................................+
void Number_simple_print(Number *nptr); // простейшая распечатка (не в научной нотации).............................................+
Number string_to_Number(const char*, int); // запись строки в число.....................................................+

/* копирование и размножение чисел */
void MOV(t_N*, t_N*); // копировать данные из правого указателя в левый.................................................+
Number copy_Number(t_N*); // создать копию уже имеющегося числа.........................................................+

/* арифметический блок */
Number ADD(t_N*, t_N*); // операция сложение
void ADD_2(t_N*,t_N*,t_N*); // операция сложения
Number SUBSTRACT(t_N*, t_N*); // операция вычитания
void SUBSTRACT_2(t_N*, t_N*, t_N*); // другое вычитание
Number MUL(t_N*, t_N*); // умножение
void MUL_2(t_N*, t_N*, t_N*); // умножение 2
Number DEVIDE(t_N*, t_N*); // деление
void DEVIDE_2(t_N*, t_N*, t_N*); // деление 2
Number POW(t_N*, t_N*); // возведение в степень
void  POW_2(t_N*, t_N*, t_N*); // возведение в степень 2


/* блок функций */
Number expN(Number*);
Number cosN(Number*);
Number sinN(Number*);
Number lnN (Number*);


#endif //NUMBER_NUMBER_H
