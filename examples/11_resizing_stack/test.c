// Copyright 2024 Vladislav Aleinik
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include <time.h>

//=================================================//
// Тестовый стенд для измерения производительности //
//=================================================//

// Тип данных, содержащихся в стеке
// Это объявление используется в файле "stack.h"
typedef uint64_t data_t;

// Добавление собственного заголовочного файла
#include "stack.h"

const unsigned BENCHMARK_STEP       =  20000000U;
const unsigned BENCHMARK_NUM_PUSHES = 200000000U;

int main(void)
{
    // Стратегии реаллокации
    StackResizePolicy policies[] =
    {
        RESIZE_SUM,
        RESIZE_MUL
    };

    const char* policy_names[] =
    {
        "Линейная реаллокация",
        "Экспоненциальная реаллокация с гистерезисом"
    };

    for (int policy_i = 0; policy_i < sizeof(policies) / sizeof(StackResizePolicy); ++policy_i)
    {
        // Печатаем имя стратегии реаллокации
        printf("=== %s ===\n", policy_names[policy_i]);

        // Инициализируем стек с соответствующей стратегией реаллокации
        struct Stack stack;
        StackRetCode ret = stack_init(&stack, policies[policy_i]);
        if (ret != STACK_OK)
        {
            printf("[ERROR] Unable to initialize stack: %s\n", stack_retcode_str(ret));
            return EXIT_FAILURE;
        }

        // Добавляем элементы в стек
        for (data_t bench_i = 0U; bench_i < BENCHMARK_NUM_PUSHES; bench_i += BENCHMARK_STEP)
        {
            // Начало измеряемого отрезка времени
            clock_t ticks_start = clock();

            for (data_t push_i = bench_i; push_i < bench_i + BENCHMARK_STEP; ++push_i)
            {
                ret = stack_push(&stack, push_i);
                if (ret != STACK_OK)
                {
                    printf("[ERROR] Unable to push element #%" PRIu64 ": %s\n", push_i, stack_retcode_str(ret));
                    return EXIT_FAILURE;
                }
            }

            // Конец измеряемого отрезка времени
            clock_t ticks_end = clock();

            double ticks_delta = ticks_end - ticks_start;
            double seconds = ticks_delta / CLOCKS_PER_SEC;

            // Печатаем время на наполнение стека элементов
            // printf("Push[%9" PRIu64 ", %9" PRIu64 "]: %10.6lfs\n",
            //     bench_i, bench_i + BENCHMARK_STEP - 1U, seconds);

            (void) seconds;
            printf("Push[%9" PRIu64 ", %9" PRIu64 "]: OK\n",
                bench_i, bench_i + BENCHMARK_STEP - 1U);
        }

        // Извлекаем все элементы из стека
        for (data_t bench_i = BENCHMARK_NUM_PUSHES; bench_i > 0U; bench_i -= BENCHMARK_STEP)
        {
            // Начало измеряемого отрезка времени
            clock_t ticks_start = clock();

            for (data_t pop_i = bench_i; pop_i > bench_i - BENCHMARK_STEP; --pop_i)
            {
                data_t popped = 0U;
                ret = stack_pop(&stack, &popped);
                if (ret != STACK_OK)
                {
                    printf("[ERROR] Unable to pop element #%" PRIu64 ": %s\n", pop_i, stack_retcode_str(ret));
                    return EXIT_FAILURE;
                }

                if (popped != pop_i - 1U)
                {
                    printf("[ERROR] Popped invalid element (got %" PRIu64 ", expected %" PRIu64 ")!\n", popped, pop_i - 1U);
                    return EXIT_FAILURE;
                }
            }

            // Конец измеряемого отрезка времени
            clock_t ticks_end = clock();

            double ticks_delta = ticks_end - ticks_start;
            double seconds = ticks_delta / CLOCKS_PER_SEC;

            // Печатаем время на извлечение элементов из стека
            // printf("Pop [%9" PRIu64 ", %9" PRIu64 "]: %10.6lfs\n",
            //     bench_i - 1U, bench_i - BENCHMARK_STEP, seconds);

            (void) seconds;
            printf("Pop [%9" PRIu64 ", %9" PRIu64 "]: OK\n",
                bench_i - 1U, bench_i - BENCHMARK_STEP);
        }

        // Освобождение ресурсов стека
        ret = stack_free(&stack);
        if (ret != STACK_OK)
        {
            printf("[ERROR] Unable to free stack: %s\n", stack_retcode_str(ret));
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
