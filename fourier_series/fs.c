//
// Created by alpharius on 28.07.19.
//



#include "fs.h"


#define FUN(ptr) \
    double fun_cos(double x){                                   \
        return 1./g_L * ptr(x) * cos(M_PI * g_n * x / g_L);     \
    }                                                           \
    double fun_sin(double x){                                   \
        return 1./g_L * ptr(x) * sin(M_PI * g_n * x / g_L);     \
    }                                                           \

// только для хоршего вывода кэффициентов ряда фурье
#define PRETTY_PRINT(ptr)\
    printf("Number of series members :%d\n", (int) ptr[0]);     \
    /* a_i from 0 to n*/                                        \
    for(int i=0; i<=ptr[0]; i++){                               \
        printf("a_%d :\t%f\n",i, ptr[1+i]);                     \
    }                                                           \
    /* b_i from 1 to n*/                                        \
    for(int i=1; i<=ptr[0]; i++){                               \
        printf("b_%d :\t%f\n", i, ptr[1+((int)ptr[0]) + i]);    \
    }                                                           \

#define  FOURIER_COEF_TEST
#undef FOURIER_COEF_TEST

static double g_L=1;
static int g_n=1;
static double (*g_f)(double);
FUN(g_f)                            // память алоцированна на статическую переменную, нам нужно лишь менять ее по необходимости

double* fourier_coef(double (*f)(double),int number_of_fs_term, struct integrate_parametrs* PARAM, enum MODS MD){
    double *result = (double*) calloc(2*number_of_fs_term+2, sizeof(double)); // 1 уже занят
    result[0] = (double) number_of_fs_term;

    g_L = (PARAM->right_limit - PARAM->left_limit)/2;
    // compute a_i coefficient from 0 to number_of...
    g_f = f;
    for(size_t i=0; i<=number_of_fs_term; i++){  // всего 1+num...
        g_n = i;
#ifdef FOURIER_COEF_TEST
        printf("param left %lf , right %lf ", PARAM->left_limit, PARAM->right_limit);
        printf("g_n=%d ", g_n);
        printf("a_%ld %lf\n",i,  integratef(fun_cos, PARAM, MD));
#endif
        result[1+i] = integratef(fun_cos, PARAM, MD);
    }

    // compute b_i coefficient from 1 to number_of...
    for(size_t i=1; i<=number_of_fs_term; i++){
        g_n = i;
#ifdef FOURIER_COEF_TEST
        printf("param left %lf , right %lf ", PARAM->left_limit, PARAM->right_limit);
        printf("g_n=%d ", g_n);
        printf("b_%ld %lf\n",i,  integratef(fun_sin, PARAM, MD));
#endif
        result[1+number_of_fs_term+i] = integratef(fun_sin, PARAM, MD);
    }
    return result;
}

// по заданным коэффициентам фурье вычисляем значение функционального ряда
double  __function_series(double *koef, double x){
    int N = (int) koef[0];
    double result= 0.;
    for(int i=N; i>=1; i--){
        result += koef[i] * cos(M_PI * i * x / g_L);
        result += koef[N+1+i] * sin(M_PI * i * x / g_L);
        //printf("g_l = %lf", g_L);
    }
    result += koef[1]/2;
    return result;
}



#define NTEST
#ifdef TEST

// gcc fs.c ../integratef/integratef.c -lm -o test

double ff(double x){
    return x;
}

int main(void){
    struct integrate_parametrs ip = {-2.,2.,0,500};
    double* r = fourier_coef(ff,10,&ip, QUADRATE);
    printf("%f", r[0]);
    PRETTY_PRINT(r);
}
#endif
