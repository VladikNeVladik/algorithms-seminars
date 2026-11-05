// Copyright 2024 Vladislav Aleinik
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//==================================================================================================
// Функция: sum_array
// Назначение: вычисляет сумму массива чисел с плавающей точкой
//--------------------------------------------------------------------------------------------------
// Параметры:
// array (in) - адрес на первый элемент массива.
// size (in) - количество элементов в массиве.
//
// Возвращаемое значение: сумма элементов массива
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// На использование функции определены следующие ограничения:
// - Память по указателю array доступна на чтение на size элементов вперёд.
//==================================================================================================
double sum_array_elements(const double* array, size_t size)
{
    // Сумма элементов массива
    double sum = 0.0;

    for (size_t i = 0; i < size; ++i)
    {
        sum += array[i];
    }

    return sum;
}

#define MAX_ARRAY_SIZE 10000U

int main(void)
{
    // Размер массива
    size_t size;

    // Считываем размер массива
    printf("Enter number of elements: ");

    int ret = scanf("%zu", &size);
    if (ret != 1)
    {
        printf("ERROR: expected a number!\n");
        return EXIT_FAILURE;
    }
    if (size > MAX_ARRAY_SIZE)
    {
        printf("ERROR: array size is too big!\n");
        return EXIT_FAILURE;
    }

    // Аллоцируем массив необходимого размера
    double elements[size];

    // Заполняем массив:
    for (size_t i = 0; i < size; ++i)
    {
        // Считываем элемент массива
        ret = scanf("%lf", &elements[i]);
        if (ret != 1)
        {
            printf("ERROR: element %zu is not a number!\n", i);
            return EXIT_FAILURE;
        }
    }

    // Вычисляем сумму элементов массива
    double sum = sum_array_elements(elements, size);

    // Выводим в консоль результат вычисления
    printf("Array sum is: %lf\n", sum);

    return EXIT_SUCCESS;
}
