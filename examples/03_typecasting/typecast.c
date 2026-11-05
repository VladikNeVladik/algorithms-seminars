// Copyright 2024 Vladislav Aleinik

#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    float operand_a = 100000000.0f;
    float operand_b =         1.0f;

    // Потеря точности, т.к. операция выполняется в 32-битном типе float.
    // Неявное преобразование типа float -> int.
    int result_with_losses = operand_a + operand_b;

    printf("(float)%f + ( float)%f = (int)%d\n",
        operand_a, operand_b, result_with_losses);

    // Нет потери точности, т.к. операция выполняется в 64-битном типе double.
    int result_lossless = operand_a + (double) operand_b;

    printf("(float)%f + (double)%f = (int)%d\n",
        operand_a, (double)operand_b, result_lossless);

    return 0;
}
