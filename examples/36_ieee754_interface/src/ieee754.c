// Copyright 2025 Vladislav Aleinik

// Запрашиваем определённую реализацию стандартной библиотеки (glibc).
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>
#include <fenv.h>

#include <signal.h>

//====================//
// Арифметика IEEE754 //
//====================//

double not_a_number()
{
    // sign     = 0
    // exponent =  111 1111 1111
    // fraction =                1000 0000  ... 0000
    uint64_t NaN = 0x7FF8000000000000LLU;

    return *((double*) &NaN);
}

bool is_nan(double x)
{
    return x != x;
}

bool is_infinity(double x)
{
    double expected_NaN = 0.0 * x;

    return !is_nan(x) && is_nan(expected_NaN);
}

//=================================//
// Вывод содержимого окружения FPU //
//=================================//

// Запрещаем компилятору предполагать биты Floating Point окружения по умолчанию.
#pragma STDC FENV_ACCESS ON

void show_fe_exceptions(void)
{
    printf("Current exceptions raised: ");

    int bitmask = fetestexcept(FE_ALL_EXCEPT);

    if (bitmask & FE_DIVBYZERO)
        printf(" FE_DIVBYZERO");
    if (bitmask & FE_INEXACT)
        printf(" FE_INEXACT");
    if (bitmask & FE_INVALID)
        printf(" FE_INVALID");
    if (bitmask & FE_OVERFLOW)
        printf(" FE_OVERFLOW");
    if (bitmask & FE_UNDERFLOW)
        printf(" FE_UNDERFLOW");
    if (bitmask == 0)
        printf(" none");
    printf("\n");
}

void show_fe_rounding_method(void)
{
    printf("Current rounding method:    ");
    switch (fegetround()) {
           case FE_TONEAREST:  printf ("FE_TONEAREST");  break;
           case FE_DOWNWARD:   printf ("FE_DOWNWARD");   break;
           case FE_UPWARD:     printf ("FE_UPWARD");     break;
           case FE_TOWARDZERO: printf ("FE_TOWARDZERO"); break;
           default:            printf ("unknown");
    };
    printf("\n");
}

void show_fe_environment(void)
{
    show_fe_exceptions();
    show_fe_rounding_method();
}

//======================//
// Обработка исключений //
//======================//

// Цвета.
#define RED     "\033[1;31m"
#define RESET   "\033[0m"

// Обработчик исключения "деление на нуль".
void handler_division_by_zero(int signum)
{
    printf(RED"[FPU EXCEPTION] Division by zero\n"RESET);
    exit(EXIT_SUCCESS);
}

void enable_division_by_zero_check()
{
    // Включаем исключение FE_DIVBYZERO.
    #ifdef FE_DIVBYZERO
        if (feenableexcept(FE_DIVBYZERO) == -1)
        {
            printf("[ERROR] Unable to enable \"division by zero\" trap\n");
            exit(EXIT_FAILURE);
        }
    #else
        printf("[BUG] Not implemented \"division by zero\" error-check on this architecture\n");
        exit(EXIT_FAILURE);
    #endif

    sigset_t block_all_signals;
    if (sigfillset(&block_all_signals) == -1)
    {
        printf("[ERROR] Unable to create filled signal mask\n");
        exit(EXIT_FAILURE);
    }

    // Устанавливаем в операционной системе Linux обработчик сигнала SIGFPE.
    struct sigaction act =
    {
        .sa_handler = handler_division_by_zero,
        .sa_mask    = block_all_signals,
        .sa_flags   = 0
    };
    if (sigaction(SIGFPE, &act, NULL) == -1)
    {
        printf("[ERROR] Unable to set \"division by zero\" handler\n");
        exit(EXIT_FAILURE);
    }
}

void disable_division_by_zero_check()
{
    // Выключаем исключение FE_DIVBYZERO.
    #ifdef FE_DIVBYZERO
        if (fedisableexcept(FE_DIVBYZERO) == -1)
        {
            printf("[ERROR] Unable to disable \"division by zero\" trap\n");
            exit(EXIT_FAILURE);
        }
    #else
        printf("[BUG] Not implemented \"division by zero\" error-check on this architecture\n");
        exit(EXIT_FAILURE);
    #endif

    struct sigaction act =
    {
        .sa_handler = SIG_IGN,
        .sa_flags   = 0
    };
    if (sigaction(SIGFPE, &act, NULL) == -1)
    {
        printf("[ERROR] Unable to disable \"division by zero\" signal\n");
        exit(EXIT_FAILURE);
    }
}

//===================================//
// Демонстрация функциональности FPU //
//===================================//

#define SAMPLE_SIZE 100
#define POW_BASE    1.1f
#define POW_STEP    0.1f
#define NUM_STEPS   10

int main(void)
{
    printf("\n");

    //==========================================================//
    // Выводим побитовое представление числа с плавающей точкой //
    //==========================================================//

    float       flt_a = 1.0;
    double      dbl_b = 1.0;
    long double ldb_c = 1.0;

    printf("flt_a = %f, sizeof = %zu\n", flt_a, sizeof(flt_a));
    printf("flt_a = ");

    uint32_t* repr_a = (uint32_t*) &flt_a;
    for (int i = 31; i >= 0; --i)
    {
        printf("%d", (*repr_a >> i) & 1);

        if (i % 8 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");

    printf("dbl_b = %lf, sizeof = %zu\n", dbl_b, sizeof(dbl_b));
    printf("dbl_b = ");

    uint64_t* repr_b = (uint64_t*) &dbl_b;
    for (int i = 63; i >= 0; --i)
    {
        printf("%lld", (*repr_b >> i) & 1);
        if (i % 8 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");

    printf("ldb_c = %Lf, sizeof = %zu\n", ldb_c, sizeof(ldb_c));

    printf("\n");

    //===============================================//
    // Производим проверки на NaN и на бесконечность //
    //===============================================//

    printf("Not a number:       %f\n", not_a_number());
    printf("Not a number check: %s\n", is_nan(not_a_number()) ? "OK" : "FAILED");
    printf("Infinity check:     %s\n", is_infinity(1.0/0.0)   ? "OK" : "FAILED");

    printf("\n");

    //============================================//
    // Демонстрируем накопление ошибки вычислений //
    //============================================//

    for (int step = 0; step < NUM_STEPS; ++step)
    {
        float base = POW_BASE + step * POW_STEP;

        printf("Sum of power series for base: %f\n", base);

        float powers[SAMPLE_SIZE] = {};
        for (size_t i = 0U; i < SAMPLE_SIZE; ++i)
        {
            powers[i] = powf(base, i);
        }

        float sum = 0.0;
        for (size_t i = 0U; i < SAMPLE_SIZE; ++i)
        {
            sum += powers[i];
        }

        printf("    Min-to-max summation:     %f\n", sum);

        sum = 0.0;
        for (int i = SAMPLE_SIZE - 1; i >= 0; --i)
        {
            sum += powers[i];
        }

        printf("    Max-to-min summation:     %f\n", sum);

        sum = (powf(base, SAMPLE_SIZE) - 1.0) / (base - 1.0);
        printf("    Analytical result:        %f\n", sum);

        printf("\n");
    }

    //=================================//
    // Работа с текущим окружением FPU //
    //=================================//

    // Отображаем в консоли текущее окружение FPU.
    show_fe_environment();
    printf("\n");

    // Производим арифметику с учётом режима округления.
    // rint() + FE_DOWNWARD   = floor()
    // rint() + FE_UPWARD     = ceil()
    // rint() + FE_TOWARDZERO = trunc()
    // rint() + FE_TONEAREST  = почти round(), но округление к чётному числу, а не "от нуля".
    printf("+11.5 -> %+4.1f\n", rint(+11.5));
    printf("-12.5 -> %+4.1f\n", rint(-12.5));
    show_fe_environment();
    printf("\n");

    // Сохраняем текущее окружение.
    fenv_t cur_env;
    fegetenv(&cur_env);

    // Производим вычисления с новым методом округления.
    feclearexcept(FE_ALL_EXCEPT);
    fesetround(FE_DOWNWARD);
    printf("1.0/0.0 = %f\n", 1.0/0.0);
    printf("+11.5 -> %+4.1f\n", rint(+11.5));
    printf("-12.5 -> %+4.1f\n", rint(-12.5));
    show_fe_environment();
    printf("\n");

    // Восстанавливаем предыдущее окружение.
    fesetenv(&cur_env);
    show_fe_environment();
    printf("\n");

    //=============================//
    // Обрабатываем исключения FPU //
    //=============================//

    // Недетектируемое деление на нуль.
    printf("1.0/0.0 = %f\n", 1.0/0.0);

    // [Linux Moment Warning]
    // Устанавливаем обработчик с помощью Linux-специфичного интерфейса.
    enable_division_by_zero_check();

    // Деление на нуль приводит к срабатыванию обработчика.
    printf("1.0/0.0 = %f\n", 1.0/0.0);

    disable_division_by_zero_check();

    return EXIT_SUCCESS;
}
