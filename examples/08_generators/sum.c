// Copyright 2024 Vladislav Aleinik
#include <stdio.h>
#include <stdlib.h>

// Конструкция typedef позволяет создавать псевдонимы типов
typedef unsigned long long ull_t;

//============================//
// Передача массива в функцию //
//============================//

#ifdef SUM_SIMPLE

ull_t sum(const unsigned* array, unsigned size)
{
    ull_t acc = 0ULL;
    for (unsigned i = 0U; i < size; ++i)
    {
        acc += array[i];
    }

    return acc;
}

#endif

//=====================================//
// Рекурсия и функции обратного вызова //
//=====================================//

#ifdef SUM_GENERATOR

ull_t sum(unsigned (*get_element)(unsigned), unsigned nmemb)
{
    if (nmemb == 0U)
    {
        return 0ULL;
    }

    return sum(get_element, nmemb - 1) + get_element(nmemb);
}


#endif

//====================//
// Хвостовая рекурсия //
//====================//

#ifdef SUM_TAIL_RECURSION
ull_t sum_helper(unsigned (*get_element)(unsigned), unsigned nmemb, ull_t acc)
{
    if (nmemb == 0U)
    {
        return acc;
    }

    return sum_helper(get_element, nmemb - 1U, acc + get_element(nmemb));
}

ull_t sum(unsigned (*get_element)(unsigned), unsigned nmemb)
{
    return sum_helper(get_element, nmemb, 0U);
}
#endif

//======//
// Цикл //
//======//

#ifdef SUM_CYCLE
ull_t sum(unsigned (*get_element)(unsigned), unsigned nmemb)
{
    ull_t acc = 0ULL;

    for (unsigned i = 0U; i <= nmemb; ++i)
    {
        acc += get_element(i);
    }

    return acc;
}
#endif

//================//
// Тестовый стенд //
//================//

unsigned generate_progression(unsigned index)
{
    return index;
}

int main(void)
{
    // Ввод количества элементов последовательности:
    printf("Enter number of sequence elements:\n");

    unsigned sequence_size;
    if (scanf("%u", &sequence_size) != 1)
    {
        printf("Expected one unsigned integer\n");
        return EXIT_FAILURE;
    }

#ifdef SUM_SIMPLE
    unsigned array[sequence_size + 1];
    for (unsigned i = 0U; i <= sequence_size; ++i)
    {
        array[i] = i;
    }

    printf("Progression sum of %u elements is: %llu\n",
        sequence_size, sum(array, sequence_size + 1));
#endif

#if defined(SUM_GENERATOR) || defined(SUM_TAIL_RECURSION) || defined(SUM_CYCLE)
    printf("Progression sum of %u elements is: %llu\n",
        sequence_size, sum(&generate_progression, sequence_size));
#endif

    return EXIT_SUCCESS;
}
