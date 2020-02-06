//
// Created by alpharius on 04.02.2020.
//

#ifndef ARBITRARY_PRECISION_NUMBER_NUMBER_H
#define ARBITRARY_PRECISION_NUMBER_NUMBER_H

/*
 * реализую тип числа с наперед заданной точностью (передавая параметр числа значащих цифр в указатель)
 *
 * amount_of_signs - дает нам границы рассмотрения памяти
 * *data - указатель на область памяти в которой хранятся байты числа
 * приведена неоптимальная (пробная реализация) в виде массива интов
 * a[0] - знак
 * a[1] - показатель степени
 * a[2..n-1] - значащие цифры по разрядам
 *
 */

typedef struct _number {
    int amount_of_signs;
    int* data;
} Number;

Number new_Number(int);                 // конструктор
Number ADD(Number*, Number*);           // сложение
Number SUBSTRACT(Number*, Number*);     // вычитание
Number MULTIPLY(Number*, Number*);      // умножение
Number DEVIDE(Number*, Number*);        // деление
double Number_to_double(Number*);       // конвертация в число с двойной точностью
Number double_to_Number(double);        // конвертация числа с двойной точностью в Number
char* Number_to_string(Number*);        // конвертация в строковое представление
Number string_to_Number(const char*, int);         // конвертация строки в Number
Number POW(Number*, Number*);           // возведение в степень
Number SQR(Number*, Number*);           // извлечение произвольного корня.
void Number_simple_print(Number*);       // простецкий вывод записанного :::::DEBUG:::::
void Number_move_right(Number*);        // сдвинуть на одну позицию вправо и поменять степень
void Number_move_right_pos(Number*, int);    // сдвинуть на сколько то позиций вправо
void Number_move_left(Number*);             // единичный сдвик в лево при условии что старший разряд 0   012 -> 120
void Number_move_left_pos(Number*, int);    // сдвинуть на сколько то позиций влево



#endif //ARBITRARY_PRECISION_NUMBER_NUMBER_H
