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

/* разряды определенные под отстойник ошибок */
#define ERROR_ORDER 10
#define MACHINE_ZERO 2


/* и количество позиций под знак и степень и одновременно и начальная позиция для нумерации цифр разряда */
#define SIGN_AND_POWER_CELL 2
/* количество итераций в алгоритме деления - еще не определил эмпирически */
#define LIMIT_OF_ITERATION 100

/*просто акронимы  - в конце должен быть undef */
#define EO ERROR_ORDER
#define MZ MACHINE_ZERO
#define SAPC SIGN_AND_POWER_CELL

#define DEBUG_n

/* декларация полезных утилит (определенных вконце) */
static void itoa(int, char*);                   // К&R - реализация
static void reverse(char* );                    // инверсия строки
static void __invers_digit_signs(Number*);      // изменения знака у разрядовых цифр
static void __init_seq_devide(Number*, Number*);// инициализация первого члена последовательности в алгоритме деления


/*              Функции             */

Number new_Number(int n_sign){
   Number result;
   result.amount_of_signs = n_sign;
   result.data = (int*) calloc(n_sign + SIGN_AND_POWER_CELL + EO, sizeof(int));
   return result;
}

void del_Number(Number* n){
    assert(n != NULL);
    n->amount_of_signs = 0;
    free(n->data);
    n = NULL;
}

Number string_to_Number(const char* str, int precision){
   printf("proof");
   assert(str != NULL);
   Number result = new_Number(precision);

   const char* ch_ptr;
   int* i_ptr;
   ch_ptr = str;
   i_ptr = &result.data[2];

   /* знаки перед точкой учтена возможность отсутствия + */
    //warning: pointer/integer type mismatch in conditional expression ликвидировать безобразие
    *ch_ptr == '-' ? *(i_ptr - 2)=-1, ch_ptr++, 1 : (*(i_ptr - 2) = +1);
    *ch_ptr == '+' ? ch_ptr++ : NULL;

    int signs_before_dot = 0;
    while(*ch_ptr != '\0' && *ch_ptr != '.' && *ch_ptr != 'e'){
        signs_before_dot++;
        *i_ptr = (int) *ch_ptr - '0';
        i_ptr++; ch_ptr++;
    }
    (*ch_ptr !='\0' && *ch_ptr !='e')? ch_ptr++ : NULL;

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
     *  перевод в научную натацию A.BCDEFeG' - до . остается только одно число
     *  from ABC.DEFeG to A.BCDEFeG'
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
    printf("amount of numbers %d \n", n->amount_of_signs);
    printf("amount of error orders %d\n ", EO);
    printf("sign \t| exp order \t| numbers \t| trash \t| \n");
    int *ptr = n->data;
    printf(" %d \t\t| ", *ptr);
    ptr++;
    printf("%d \t\t\t| ", *ptr);
    ptr++;
    for(size_t i=0; i<n->amount_of_signs; i++){
        printf("%d", *ptr);
        ptr++;
    }
    printf(" \t| ");
    /* print trash from EO */
    for(size_t i=0; i<EO; i++){
        printf("%d", *ptr);
        ptr++;
    }

    printf("\n");
}

void Number_move_right(Number* n){
    /*
     *  хороший вопрос! а должен ли я делать перемещения
     *  и вообще операции с мусорными числами.
     */
    int aos = n->amount_of_signs + SAPC + EO;
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
    int aos = n->amount_of_signs + SAPC + EO;
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
    if(first->data[1] != second->data[1]){
        if (first->data[1] < second->data[1]){
            Number_move_right_pos(first, second->data[1]-first->data[1]);
        } else {
            Number_move_right_pos(second,first->data[1]-second->data[1]);
        }
    }

#ifdef DEBUG
    Number_simple_print(first);
    Number_simple_print(second);
#endif
    Number result = new_Number(first->amount_of_signs);

    // сложение двух положительных чисел (точнее чисел с равными знаками)
    if (first->data[0]*second->data[0] == 1){
        result.data[0] = first->data[0];
        result.data[1] = first->data[1];

        // суммирование без учета переполнения разряда
        for(size_t i=0; i<result.amount_of_signs+EO; i++){
            result.data[SAPC + i] = first->data[SAPC+i] +
                    second->data[SAPC + i];
        }

        // учет переполнения разряда
        for(size_t i=SAPC + result.amount_of_signs + EO-1; i>1; i--){
            result.data[i - 1] += result.data[i] / 10;
            result.data[i] %= 10;
        }

        if (result.data[SAPC] >= 10){
            Number_move_right(&result);
            result.data[SAPC] = result.data[SAPC+1] / 10;
            result.data[SAPC+1] %= 10;
        }
    } else {
        /*
         *  Алгоритм такой - из большего мы вычитаем меньшее и устанавливаем нужный знак
         */
        Number *big, *small;    // проверка по абсолютной величине
        Number *negative, *positive;    // указатель на положительное и отрицательное число
        int sign;               // знак окончательного ответа
        negative = first->data[0] == -1? first : second;
        positive = first->data[0] == -1? second : first;

        /* проверка на то, модуль какого числа больше - положительного или отрицательного
           для этого меняем знак у негативного числа и сравниваем его с позитивным, в случае
           если модуль отрицательного числа больше - то делаем инверсию знаков - что бывшее положительное
           число становится отрицательным а бывшее отрицательное положительным, но это изменение должно
           учитываться в знаке конечного результата

           В итоге мы всегда вычитаем из болшего числа меньшее, а появившиеся отрицательные числа в разря
           дах мы компенсируем уменьшением числа следующего разряда на 1 и прибавление 10ки к отрицательно
           му числу в текущем разряде.
           a,b,-c => a,b-1,10-c


         */
        negative->data[0] = +1;
        if (LE(negative, positive)){
            sign = +1;
            big = positive;
            small = negative;
        } else {
            sign = -1;
            big = negative;
            small = positive;
        }

        __invers_digit_signs(small);
        // сложение без учета разрядов
        for(size_t i=0; i<SAPC + first->amount_of_signs + EO; i++){
            result.data[i] = big->data[i] + small->data[i];
        }

        // учет наличия отрицательных чисел в разряде
        // пробегаем от последнего числа к [3]му - [2]е мы не можем трогать - дальше только степень
        for(size_t i=SAPC + first->amount_of_signs-1 + EO; i>SAPC; i--){
            if (result.data[i] < 0){
                result.data[i] += 10;
                result.data[i-1] -= 1;
            } /* в противном случае ничего не меняем */
        }
        assert(sign != 0);
        result.data[0] = sign;
        result.data[1] = first->data[1];
    }

    while(result.data[SAPC] == 0 && result.data[1] > 0){
        Number_move_left(&result);
    }
    return result;
}

Number SUBSTRACT(Number* first, Number* second){
    /*
     * first - second
     */
    second->data[0] *= -1;
    return ADD(first, second);
}

Number MULTIPLY(Number* first, Number* second){
    /*
     * знак - произведение знаков
     * показатели степени - складываются арифметически
     * разряды вычисляются следующим образом разряд начинается с [2]
     *
     * $ c_{k+2} = \sum_{l=0}^k a_{k-l+2} b_{l+2} $
     * k from 0 to n-2 - дальше просто борюсь с переполнением разрядов
     * и в случае необходимости сдвигаем разряды как должно
     *
     * если с нужной реперной точки
     * $ c_{k} = \sum_{l=0}^k a_{k-l} b_{l} $
     */
    assert(first->amount_of_signs == second->amount_of_signs);
    Number result = new_Number(first->amount_of_signs);
    result.data[0] = first->data[0] * second->data[0];
    result.data[1] = first->data[1] + second->data[1];

    // вычисление столбиком по формуле
    for(size_t k=0; k<result.amount_of_signs + EO; k++){
        int intermediate_sum = 0;
        for(size_t l=0; l<=k; l++){
            intermediate_sum += first->data[SAPC+k-l]*second->data[SAPC+l];
        }
        result.data[SAPC+k] = intermediate_sum;
    }

    // уничтожаем переполнение
    for(size_t k=result.amount_of_signs + EO -1; k>0; k--){
        if(result.data[SAPC + k] >= 10){
            int intermediate = result.data[SAPC+k] / 10;
            result.data[SAPC+k] = result.data[SAPC+k] % 10;
            result.data[SAPC+k-1] += intermediate;
        }
    }

    // уничтожаем переполнение в первом-старшем разряде
    while(result.data[SAPC]>10){
        Number_move_right(&result);
        result.data[SAPC] = result.data[SAPC+1] % 10;
        result.data[SAPC+1] = result.data[SAPC+1] / 10;
    }
    return result;
}

Number DEVIDE(Number* first, Number* second){
    /*
     *  first / second = first * sign(second) * (1/\y\)
     *
     *  используем Ньютонов алгоритм
     *  $ x_{n+1} = 2*x_{n} - x_{n}*x_{n}*\y\ . тогда  x_n => 1/\y\
     *
     *  в качестве x_1 используем приблежение первых четырех знаков выписываем первые 4 знака числа на
     *  которые делим 10 000 000 / abcd = a'b'c'd'
     *  x1 = a'b'c'd' ** (-pow(y)-1)
     *  продолжаем проводить итерацию до момента ABS(x_{n+1} - n_{n}) <  сколько то предпоследний знаков
     *  в виду неопределенной точности подаваймых чисел - следует жостко привязать критерий остановки
     *
     *  Цитата:
     *  Если мы взяли 10 запасных значащих цифр, то обрывать итерацию следует при совпадении
     *  ... с точностью до 5 последних знаков. Дело в том, что последние значащие цифры не являются
     *  верными (они содержат ошибки округления). (Численные методы для физиков теоретиков)
     *
     *  Замечание - предполагаю точность большую чем 16 знаков - (иначе нет смысла использовать Числа
     *  когда можно ограничется использованием double) я бы сказал, что хорошо бы было бы начинать то
     *  чность с 26 значаших знаков. Чтобы 10 знаков были дополнительными - тогда последние 5 содержат
     *  ошибку округления
     *  Замечание 2 - пока предположим, что ошибки округления будут занимать 5 разрядов, тогда
     *  при постоновке задач, требуется вводить число разрядов, как приемлемый порядок точности
     *  плюс 10 разрядов, последние пять из которых содержали бы в себе ошибку округления.
     */
    assert(first->amount_of_signs == second->amount_of_signs);
    Number result = new_Number(first->amount_of_signs);
    Number X_n, X_np1;  // x_{n} x_{n+1}
    Number Deltha;
    X_n = new_Number(first->amount_of_signs);
    X_np1 = new_Number(second->amount_of_signs);

    // инициализация X_n - по алгоритму из книги
    __init_seq_devide(&X_n, second);
    printf("\nXn = %s\n",Number_to_string(&X_n));

}

void ABS(Number* n){
    n->data[0] = +1;
}

Number Number_copy(Number* copied){
    Number result = new_Number(copied->amount_of_signs);
    for(size_t i=0; i<SAPC + copied->amount_of_signs + EO; i++){
        result.data[i] = copied->data[i];
    }
    return result;
}



bool EQUAL(Number* first, Number* second){
    bool result = true;
    result = result && first->amount_of_signs == second->amount_of_signs;
    result = result && first->data[0] == second->data[0] && first->data[1] == second->data[1];
    if (!result) return result;

    // проверка с точностью до последнего числа в разряде дальше идет помойка чисел
    // машинный ноль начинается через 2 знака после значащих разрядов
    for(size_t i=SAPC; i<SAPC + first->amount_of_signs + MZ ; i++){
        result = result && first->data[i] == second->data[i];
        if (!result) return result;
    }
    return result;
}

bool LE(Number* first, Number* second){
    /*
     *   first <= second
     *   хорошенько это надо проверить
     */

    assert(first->amount_of_signs == second->amount_of_signs);
    // проверка на разные знаки - после этого полагаем знаки равными
    if (first->data[0] == -1 && second->data[0] == +1) return true;
    if (first->data[0] == +1 && second->data[0] == -1) return false;

    if(EQUAL(first, second)) return true;

    if (first->data[1] == second->data[1]) goto equal_power;

    // теперь когда знаки одинаковы - мы должны определить, кто по модулю больше, а дальше анализировать знаки
    // вначале проверяем на порядок - число с большим порядком - болше по модулю
    // после этого мы полагаем показатель экспоненты равными
    if (first->data[1] < second->data[1] && first->data[0] == +1) return true;
    if (first->data[1] > second->data[1] && first->data[0] == -1) return true;
    if (first->data[1] > second->data[1] && first->data[0] == +1) return false;
    if (first->data[1] < second->data[1] && first->data[0] == -1) return false;
    abort();

    // остается только рассмотреть когда у нас порядки одинаковые
    equal_power:
    if (first->data[0] == +1){
        for (size_t i=0; i<SAPC + first->amount_of_signs + MZ; i++){
            if (first->data[i] < second->data[i]){
                return true;
            }
            if (first->data[i] > second->data[i]){
                return false;
            }
        }
    } else {
        for (size_t i=0; i<SAPC + first->amount_of_signs + MZ; i++){
            if (first->data[i] > second->data[i]){
                return true;
            }
            if (first->data[i] < second->data[i]){
                return false;
            }
        }
    }
    return true; // просто чтобы выйти
}

bool GE(Number* first, Number* second){
    /*
     * first >= second;
     */
    return LE(second, first);
}



/*                  утилиты                  */
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

// микро утилиты
static void __invers_digit_signs(Number* n){
    for(size_t i=0; i<SAPC + n->amount_of_signs + EO; i++){
        n->data[i] *= -1;
    }
}

static void __init_seq_devide(Number* n, Number *second){
    /*
     * алгоритм из книги "Численные методы для физиков теоретиков"
     */
    int sum=0;
    for(int i=0; i<4; i++){
        sum = 10*sum + second->data[SIGN_AND_POWER_CELL+i];
    }
    int aproximation = 10000000/sum;
    for(int i=0; i<4; i++){
        n->data[5-i] = aproximation % 10;
        aproximation /= 10;
    }
    n->data[0] = +1;
    n->data[1] = -second->data[1] - 1;
}