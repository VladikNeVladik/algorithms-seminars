// Copyright 2024 Vladislav Aleinik
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE_X 40
#define SIZE_Y 20

int main(void)
{
    // Инициализируем массивы.
    double array1d[SIZE_Y * SIZE_X];
    double array2d[SIZE_Y][SIZE_X];

    for (size_t y = 0U; y < SIZE_Y; ++y)
    {
        for (size_t x = 0U; x < SIZE_X; ++x)
        {
            array1d[y * SIZE_X + x] = (double) (x + y);
            array2d[y][x] = (double) (x + y);
        }
    }

    // Вычисляем сумму элементов массива.
    double sum1d = 0.0;
    double sum2d = 0.0;

    for (size_t y = 0U; y < SIZE_Y; ++y)
    {
        for (size_t x = 0U; x < SIZE_Y; ++x)
        {
            sum1d += array1d[y * SIZE_X + x];
            sum2d += array2d[y][x];
        }
    }

    // Выводим в консоль результат вычисления
    printf("1D array sum is: %lf\n", sum1d);
    printf("2D array sum is: %lf\n", sum2d);

    return EXIT_SUCCESS;
}
