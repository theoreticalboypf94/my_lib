//
// Created by alpharius on 26.08.19.
//
/*
 * gcc lib.c -shared -fPIC -o lib.so
 *
 * // in main.c
 * #include <dlfcn.h>
 * void* handle = dlopen("./lib.so",RTLD_LAZY);
 * void* f = dlsym(handle, "your_f_name(symbol)");
 * (cast f to function pointer with right signature)
 *
 *  and then (when we have "dl.so" file) we can compile
 *  our main.c with linking it:
 *  gcc main.c -ldl // -ldl -/dinamic linking/ aka dl.so
 *
 *  // avoid itoa
 *  http://www.strudel.org.uk/itoa/
 */



#include "ode_RK_solutor.h"
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>


#define CHAR_BUFFER 10000
#define FILENAME 80
#define COMPILER 100
#define STATEMENT 100
#define INCLUDE_HEADERS "#include <stdio.h>\n"\
"#include <stdlib.h>\n"



#define FUNCTION_SIGNATURE "typedef double (* _fp)(double); \n"\
"void RK_4_solutor(double _from, double _to, double h, double *init_condition, _fp terms[]){\n"


/*
 *  y'' + F(x,y)y' + G(x,y)y = 0;
 *   equal to
 *   sistem | y' = z                     = f(x,y,z)
 *          | y'' = F(x,y)z + G(x,y)y    = g(x,y,z)
 *   y_n+1 = y_n + h/6 ( K0 + 2K1 + 2K2 + K3)
 *   z_n+1 = z_n + h/6 ( L0 + 2L1 + 2L2 + L3)
 *
 *   K0 = f(x_n, y_n, z_n)
 *   K1 = f(x_n + h/2, y_n + K0*h/2, z_n + L0*h/2)
 *
 * в итоге:
 *  1) ножно y_i где i порядок производной
 *  2) нужно K_i_(0,1,2,3) - n штук по n+1 аргументу, где n- порядок уравнения
 *
 */

void ode_RK_solutor(size_t order, funp terms[], const struct t_rk_parametrs* integrate_limits ,double* init_condition){
    char text[CHAR_BUFFER]; //(char*) calloc(CHAR_BUFFER, sizeof(char));
    char filename[FILENAME];// = "pass";
    char compiler[COMPILER];// = "gcc RKRESULT.c -shared - fPIC -o RESULT";

    snprintf(filename, FILENAME, "./RK4/RK_4_solutor_order_%ld.c", order);
    snprintf(compiler, COMPILER, "gcc ./RK4/RK_4_solutor_order_%ld.c -shared -fPIC -o RESULT", order);

    char statement[STATEMENT];
    strcat(text, INCLUDE_HEADERS);

    // add right function pointer typedef
    strcat(text, "// pointer for systemm filleing\n");
    sprintf(statement,"typedef double (*_FP)(");
    strcat(text, statement);
    for(int i=0; i<order+1; i++){
        if(i == order){
            strcat(text, "double* );\n");
        } else {
            strcat(text,"double*, ");
        }

    }
    strcat(text, FUNCTION_SIGNATURE);

    // initialize FILE variables
    // сюда мы будем записывать эволюцию решения и всех ее производных
    for(int i=0; i<order; i++){
        snprintf(statement, STATEMENT, "\tFILE *fp_%d = fopen('solution_ode_order=%d.txt');\n", i, i);
        strcat(text, statement);
    }

    // начало математики
    strcat(text, "\tsize_t N = (size_t) (_to - _from)/h\n");
    strcat(text, "\tdouble X[N]");
    // форируем массивы для записи отчетов по производными
    for(int i=0; i<order; i++){
        if (i == order-1){
            sprintf(statement,",Y_order_%d[N];\n", i);
        } else {
            sprintf(statement, ", Y_order_%d[N]", i);
        }
        strcat(text, statement);
    }
    // запись коэффициентов метода рунге-кунны 4 столбца и order столбцов
    for(int j=0; j<order; j++){
        strcat(text, "double ");
        for(int i=1; i<5; i++){
            sprintf("K_order_eq_%d__%d", j, i);
        }
    }


    //strcat(text, "\t double x[]")
    for(int i=0; i<order; i++){

    }



    // close FILE variables
    for(int i=0; i<order; i++){
        snprintf(statement, STATEMENT, "\tfclose(fp_%d);\n", i);
        strcat(text, statement);
    }

    strcat(text, "}\n");

    printf("%s",text);
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "%s",text);
    fclose(fp);

    //system(compiler);
}

#undef FILENAME
#undef CHAR_BUFFER
#undef COMPILER



#define TEST
#ifdef TEST

funp g_fp[] = {sin, cos};
double g_init[] = {1,2};
struct t_rk_parametrs g_param = {0, 1, 0.01};

int main(void){
    ode_RK_solutor(4, g_fp, &g_param, g_init);
    return 0;
}
#endif