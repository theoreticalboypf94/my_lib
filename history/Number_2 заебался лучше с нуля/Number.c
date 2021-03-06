//
// Created by alpharius on 02.04.2020.
//

#include "Number.h"
#define AMOUNT nptr->amount_of_signs /* бич макрос */
#define SIGN nptr->data[0]
#define POWER nptr->data[1]
#define NUMBERS &nptr->data[2]  /* указатель на цифры в числе */
#define ABS(x) (((x) >= 0) ? x : -x)  /* вроде не должно хуярить */
#define SIGN_OF_INT(x)  (((x) > 0) ? +1 : -1)
/* типичная нотация единичного элемента - нейтральное сложение */
#define INIT_I_UNIT(S) Number I = new_Number(S); I.data[0]=1; I.data[1]=0; I.data[2] = 1
#define INIT_ZERO(S) Number Z = new_Number(S)  /* новая нотация для нулевого элемента */

#define POWER_INDEX 1  /* индекс на котором находится показатель степени */
#define SIGN_AND_POWER 2
#define SAP SIGN_AND_POWER
#define MAX_AMOUNT_OF_NUMBERS_IN_INT 11

/*
 * декларация методов-утилит, полезных внутри других функций
 */
int _number_not_zero_elements(Number *nptr);
static void _account_of_signs_in_numbers(Number*, Number*);
static void _change_sign(Number*);
static void position_addition(Number*, Number*);
static void position_alignment(Number*);
static void __ZATRAVKA(Number*, Number*);
static bool prove_Number_order_DBG(Number*);

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
    assert(nptr->data[SAP] == 0); // защита от потерии информации
    POWER--;
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
    POWER++;
    for(size_t i= AMOUNT + NUMBER_ERROR - 1; i > 0; i--){
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
     *
     *  PS - память, которая аллоцируется тут - статичная а значит ты можешь только один раз вызывать функцию с
     *  printf - потому что тогда printf будет выводить всякий мусор - потому что при следующим вызове Number_to_string
     *  исходная алоцированная память будет очищена и занята под новую строчку, в то время как принт эф вначале ждет
     *  передачу всех трех адресов на память для вывода, а этого не произойдет. Аналогично, нельзя давать char* на
     *  повторные вызовы NtSt иначе будут указывать на мусор, выход в использовании strcat или чего то в этом роде.
     */

    static char*  result; // сделал указатель крепостным этой функции
    // ??? спасет ли это меня от утечки памяти ???
    if (result != NULL){
        free(result);
    }
    result = calloc(1+1+1+AMOUNT+MAX_AMOUNT_OF_NUMBERS_IN_INT,sizeof(char));
    result[0] = SIGN==+1 ? '+' : '-';
    result[1] = nptr->data[2] + '0';
    result[2] = '.';
    int limit = _number_not_zero_elements(nptr);
    for(int i=0; i<limit; i++){
        result[3+i] = nptr->data[3+i] + '0';
    }
    if (limit == 0) {
        result[3] = '0';
    }
    result[3+limit] = 'e'; // 5+limit дальше
    char *tmp_ch_ptr = calloc(11, sizeof(char));
    itoa(POWER, tmp_ch_ptr);
    strcat(result, tmp_ch_ptr);
    free(tmp_ch_ptr);

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
     *
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
                    power_modificator++;
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
    out_of_finit_automat:
    POWER += power_modificator;
    return result;
}

// копирование и размножение чисел [1]
void MOV(Number* left, Number* right){
    /*
     * копирует содержимое правого указателя в левый
     * аналогично мнемонике mov в ассеблере
     */
    assert(left->amount_of_signs == right->amount_of_signs);
    for(size_t i=0; i< SAP + left->amount_of_signs + NUMBER_ERROR; i++){
        left->data[i] = right->data[i];
    }
}
Number copy_Number(Number* _copy){
    /*
     * репликация числа идущая рука об руку с созданием нового
     */
    Number result = new_Number(_copy->amount_of_signs);
    for(size_t i=0; i< SAP + _copy->amount_of_signs + NUMBER_ERROR; i++){
        result.data[i] = _copy->data[i];
    }
    return result;
}

// арифметический блок
void ADD_ptr(Number* result, Number* first, Number* second){
    /*
     *  сложение левого и правого элемента и запись в первый аргумент (который мы полагаем нулю)
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

    assert(result != NULL);
    assert(result->amount_of_signs != 0);
    assert(result->data != NULL);
    ZEROFICATION(result);

    // приравниваем показатели степени
    while(first->data[1] != second->data[1])
        (first->data[1] < second->data[1]) ? move_right(first) : move_right(second);
    _account_of_signs_in_numbers(first, second);
    // в result - занесена поразрядовая сумма
    // возможно с перегрузками по разрядам, которую мы должны ликвидировать
    position_addition(result, first);
    position_addition(result, second);
    result->data[POWER_INDEX] = first->data[POWER_INDEX];
    position_alignment(result);
}
Number ADD(Number* first, Number* second){

    Number result = new_Number(first->amount_of_signs);
    ADD_ptr(&result, first, second);

    return result;
}

void SUBSTRACT_ptr(Number* result, Number* first, Number* second){
    /*
    * result = first - second
    * NB - без [1] я по сути реализовываю операцию
     * result += first - second
    */
    assert(result != NULL);
    assert(result->amount_of_signs != 0);
    assert(result->data != NULL);
    ZEROFICATION(result);
    second->data[0] *= -1;
    ADD_ptr(result, first, second);
    second->data[0] *= -1; // возвращаем знак к исходному
}
Number SUBSTRACT(Number* first, Number* second){
    /*
     * result = first - second
     */

    Number result = new_Number(first->amount_of_signs);
    SUBSTRACT_ptr(&result, first, second);
    return result;
}

void MUL_ptr(Number* result, Number* first, Number* second){
    /*
     * умножение реализуется по следующему алгоритму из книги "численные методы для физиков теоретиков"
     * $$ C_{k+2} = \sum_{l=0}^{k} a_{k-l+2} * b_{l+2} $$ k from 2 to n-1
     *
     * важно, что точность всех аргументов должна совпадать
     */
    assert(first->amount_of_signs == second->amount_of_signs);
    assert(result->amount_of_signs == first->amount_of_signs);
    assert(result != NULL);
    assert(result->amount_of_signs != 0);
    assert(result->data != NULL);

    ZEROFICATION(result); // добавил во избежание ошибки result += first * second

    result->data[0] = first->data[0] * second->data[0];
    result->data[1] = first->data[1] + second->data[1];

    for(size_t k=0; k < SAP+result->amount_of_signs+NUMBER_ERROR-2; k++){
        int com_sum = 0;
        for(size_t l=0; l<=k; l++){
            com_sum += first->data[k-l+SAP] * second->data[l+SAP];
        }
        result->data[SAP+k] = com_sum;
    }

    // ликвидация порарядовой перегрузки
    for(size_t k=SAP+result->amount_of_signs+NUMBER_ERROR-1; k>SAP; k--){
        result->data[k-1] += result->data[k] / 10;
        result->data[k] = result->data[k] % 10;
    }

    // ликвидация перегрузки в последнем разряде
    while(result->data[SAP] > 9){
        move_right(result);
        result->data[SAP] = result->data[SAP+1] / 10;
        result->data[SAP+1] = result->data[SAP+1] % 10;
    }
}
Number MUL(Number* first, Number* second){
    /*
     * умножение с возвратом нового числа
     */
    assert(first->amount_of_signs == second->amount_of_signs);
    Number result = new_Number(first->amount_of_signs);
    MUL_ptr(&result, first, second);
    return result;
}

void DEVIDE_ptr(Number* result, Number* first, Number* second){
    /*
     *  Алгоритм взят из книги "Численные методы для физиков теоретиков", там реализован алгоритм длеления по методу
     *  Ньютона Рапсона, сходимость к плавильному ответу квадратичная, каждый следующий щаг приносит 2 новых точных
     *  знака, важно, что нулевой (затравочный) элемент должен быть меньше, точки сходимости, но это и так понятно
     *  иначе бесконечный рост - гарантирован
     *
     *   result = first / second = first * sign(second) * (1/\y\)
     *   $$ x_{n+1} = 2*x_{n} - x_{n}*x_{n}*\y\ $$. тогда  x_n => 1/\y\
     *   x_np1 = tmp1 - tmp3
     *
     *  в качестве x_1 используем приблежение первых четырех знаков выписываем первые 4 знака числа на
     *  которые делим 10 000 000 / abcd = a'b'c'd'
     *  x1 = a'b'c'd' ** (-pow(y)-1)
     *  продолжаем проводить итерацию до момента (x_{n+1} == n_{n}) или до исчерпания количества итераций - которых
     *  нужно примерно в два раза меньше чем количество разрядов
     *
     * Цитата:
     *  Если мы взяли 10 запасных значащих цифр, то обрывать итерацию следует при совпадении
     *  ... с точностью до 5 последних знаков. Дело в том, что последние значащие цифры не являются
     *  верными (они содержат ошибки округления). (Численные методы для физиков теоретиков)
     *
     *
     *  {1} - я просто был вынужден наплодить такое количество переменных, текущие реализации алгоритмов деления
     *  не допускают выражений вида a = a*b - нужно вводить новую переменную, методы которые не используют указатель
     *  для записи первого аргумента я не применяю - чтобы не получить утечки памяти (а это ой как легко)
     *  Вообще говоря MUL DEVIDE - и прочие без _ptr являются небезопасными.
     *
     *  {2} - равенство в смысле совпадения всех разрядов, кроме последних ERROR_NUMBER 5 разрядов отведенных под ошибки
     *
     *  {3} - ошибка заключалась в том что я не очень грамотно реализовал функцию деления (да как и прочие функции)
     *  дело в том, что по сути я реализовал операцию SUBSTRACT_ptr(*result, *first, *second) полагая, что реализо
     *  вал операцю result = first - second; в то время, как на самом деле была реализована операция
     *  result += first - second
     *
     */
    assert(first->amount_of_signs == second->amount_of_signs);
    assert(result->amount_of_signs == first->amount_of_signs);
    assert(result != NULL);
    assert(result->amount_of_signs != 0);
    assert(result->data != NULL);
    ZEROFICATION(result);           // все то же самое стримимися избежать ошибки result += first / second

    Number X_n, X_np1;
    Number tmp1, tmp2, tmp3;  // {1}
    X_n = new_Number(first->amount_of_signs);   X_n.data[0]   = 1;
    X_np1 = new_Number(first->amount_of_signs); X_np1.data[0] = 1;
    tmp1 = new_Number(first->amount_of_signs);  tmp1.data[0]  = 1;
    tmp2 = new_Number(first->amount_of_signs);  tmp2.data[0]  = 1;
    tmp3 = new_Number(first->amount_of_signs);  tmp3.data[0]  = 1;
    Number Y = copy_Number(second);                 Y.data[0] = 1;

    __ZATRAVKA(&X_n, &Y);
    size_t counter=0, number_of_iteration = first->amount_of_signs / 2 + 5;
    goto inside_loop;
    while (!EQUAL(&X_n, &X_np1) && counter++ < number_of_iteration){        /* {2} */
        MOV(&X_n, &X_np1);
        inside_loop:;
        ADD_ptr(&tmp1, &X_n, &X_n);
        MUL_ptr(&tmp2, &X_n, &X_n);
        MUL_ptr(&tmp3, &tmp2, &Y);
        SUBSTRACT_ptr(&X_np1, &tmp1, &tmp3);   /* {3} */

        // зануление временных переменных - иначе мы получаем ошибки
        ZEROFICATION(&tmp1);
        ZEROFICATION(&tmp2);
        ZEROFICATION(&tmp3);
    }
    X_np1.data[0] = second->data[0];    // приравнивание знаков, для совпадения
    MUL_ptr(result,first, &X_np1);
}
Number DEVIDE(Number* first, Number* second){
    assert(first->amount_of_signs == second->amount_of_signs);
    Number result = new_Number(first->amount_of_signs);
    DEVIDE_ptr(&result, first, second);
    return result;
}

void SQR_ptr(Number* result, Number* first, Number* second){
    /*
     * реализация алгоритма по возведению в произвольную степень
     * $$ X_np1 =  (1/N) * ( (N+1)*X_n - X_n**(N+1)*y)$$    => 1/ N root(y) в конце надо единицу поделить на это число
     * $$ result = left_devision ( internal_sum*X_n - internal_multiplication*internal_sum*y)
     * N - как я понимаю - целое число - иначе операция X_n**(N+1) - теряет всякий смысл на данном шаге рассмотрения
     * result = first ** second
     */
    assert(_number_not_zero_elements(second) - second->data[1] < 0); // проверку на целочисленность поставленного числа 1.23 & 1.23e2
    ZEROFICATION(result);
    INIT_I_UNIT(first->amount_of_signs);
    INIT_ZERO(first->amount_of_signs);
    Number X_n, X_np1;
    X_n = new_Number(first->amount_of_signs);
    X_np1 = new_Number(first->amount_of_signs);
    size_t number_of_iterations = first->amount_of_signs / 2;
    Number left_division = new_Number(first->amount_of_signs);
    Number Internal_sum = new_Number(first->amount_of_signs);
    Number internal_multiplication = new_Number(first->amount_of_signs);

    /* (1/N) */
    DEVIDE_ptr(&left_division, &I, second);
    /* (N+1) */
    ADD_ptr(&Internal_sum, &I, second);


}


// блок знаков неравенства
bool EQUAL(Number* first, Number* second){
    assert(first->amount_of_signs == second->amount_of_signs);
    bool result = true;
    for(size_t i=0; i<SAP+first->amount_of_signs; i++){
        result = result && (first->data[i] == second->data[i]);
        if (!result)
            return result;
    }
    return result;
}

// вспомогательный блок
void ZEROFICATION(Number* nptr){
    /*
     *  предполагаю, что знак у нуля +
     */
    assert(nptr != NULL);
    for(size_t i=0; i<nptr->amount_of_signs + NUMBER_ERROR; i++){
        nptr->data[SAP + i] = 0;
    }
    nptr->data[1] = 0;
    nptr->data[0] = +1;
}
void int_to_Number(Number* nptr, int N){
    int save_length = nptr->amount_of_signs;
    del_Number(nptr);
    char str[15];
    itoa(N,str);
    *nptr = string_to_Number(str, save_length);
}


// место для утилит

int _number_not_zero_elements(Number *nptr){
    /*
     * 1.23000 - должен дать 2
     */
    int counter=AMOUNT;
    for(int i=AMOUNT; i>1; i--){
        if (nptr->data[SAP + i -1]!=0){
            break;
        }
        counter--;
    }
    return counter-1;
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

static bool prove_Number_order_DBG(Number* nptr){
    /*
     *  функция которая проверяет нет ли у нас перегрузки по раздрядам - на тот случай если в разряд к нам
     *  записанно число больше 9
     *  true - ошибочных разрядов в числе нет
     *  false - ошибки присутствуют
     */
    bool result = true;
    for(size_t i=0; i<nptr->amount_of_signs + NUMBER_ERROR; i++){
        result = result && (nptr->data[SAP + i] <10);
        if (!result) return result;
    }
    return result;
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
     * first += second
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
            nptr->data[i-1] += position / 10;
        }
    }

    // ликвидация перегрузки старшего разряда - возможно стоит объединить
    if(ABS(nptr->data[SAP]) >= 10){
        move_right(nptr);
        nptr->data[SAP] = nptr->data[SAP+1] / 10;
        nptr->data[SAP+1] %= 10;
    }

    // ликвидация нулевого старшего разряда
    int counter = 0;    // защита от получения бесконечного цикла из за нулевого результата суммирования
    while (nptr->data[SAP] == 0 && counter++ < nptr->amount_of_signs){
        move_left(nptr);
    }
    if (counter == nptr->amount_of_signs) goto finish;  // оптимизационный прыжек на случай нуля - но это хуйня.

    // с этого мы гарантируем то что старший разряд отличен от нуля - нотация сохранена.

    //установка правильного знака - определяется старшим разрядом поразрядового суммирования
    nptr->data[0] = (nptr->data[SAP] >=0) ? +1 : -1;

    // приведение к однозначному выражению каждый разряд либо только + либо только -
    // прибавление(вычитание) подстраивается под знак старшего разряда
    for(size_t i=SAP + nptr->amount_of_signs + NUMBER_ERROR; i>SAP; i--){
        if (nptr->data[SAP] * nptr->data[i] < 0){
            nptr->data[i] += 10 * SIGN_OF_INT(nptr->data[SAP]);
            nptr->data[i-1] -= 1 * SIGN_OF_INT(nptr->data[SAP]);
        }
    }

    // повторная ликвидация нулевого старшего разряда
    counter = 0;    // защита от получения бесконечного цикла из за нулевого результата суммирования
    while (nptr->data[SAP] == 0 && counter++ < nptr->amount_of_signs){
        move_left(nptr);
    }
    // если знак отрицательный то нужно - привести разряды в беззнаковую форму с переносом знака в нужное место
    if (nptr->data[SAP] <0){
        // разряды состоят только из отрицательных и нулевых значений
        for(size_t i=SAP; i<SAP+nptr->amount_of_signs+NUMBER_ERROR-1; i++){
            nptr->data[i] *= -1;
        }
    }
    finish:;
}
// END ADD_UTILS

// BEGIN DEVIDE_UTILS
static void __ZATRAVKA(Number* init_seq_el, Number* second){
    /*
     *  затравочный элемент  для последовательности - на основании "численные методы для физиков теоритеков
     *
     *  NB. затравка изложенная в книге - не правильная - они адют числа которые являются больше ответа,
     *  а это разрушает алгоритм ньютона-рапсона из книги, модернизации я помечу [mod]
     */
    int denumenator = second->data[SAP]*1000 + second->data[SAP+1]*100 + second->data[SAP+2]*10 + second->data[SAP+3]*1;

    init_seq_el->data[0] = +1;
    init_seq_el->data[1] = - second->data[1] - 1;
    int zatravka = 10000000 / (denumenator + 1)  -1;        // mod
    assert(zatravka != 0);
    // порязрядовая запись  в две комманды - по этому отход от
    init_seq_el->data[SAP] = zatravka / 1000;    zatravka -= 1000 * (zatravka / 1000);
    init_seq_el->data[SAP+1] = zatravka / 100;   zatravka -= 100 * (zatravka / 100);
    init_seq_el->data[SAP+2] = zatravka / 10;    zatravka -= 10 * (zatravka / 10);
    init_seq_el->data[SAP+3] = zatravka / 1;     zatravka -= zatravka / 1;

    // просто страховка
    if (init_seq_el->data[SAP] == 0)
        move_left(init_seq_el);
}
// END DEVIDE UTILS


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


/*
 *  [1] - сделана как попытка избежания порочной практики использования оператора присваивания, когда будет
 *      битовое копирование указателя, но не будет соответствующего изменения памяти, мы просто получим вто
 *      рой псевдоним области памяти, что просто паршиво.
 */