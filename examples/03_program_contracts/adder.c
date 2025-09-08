// Copyright 2024 Vladislav Aleinik

#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

int main(void)
{
    short num1, num2 = 0;

    // Производим разбор ввода в программу.
    int num_inputs = scanf("%hd\n%hd", &num1, &num2);
    if (num_inputs < 2)
    {
        printf("adder: expected input \"<addentum1>\\n<addentum2>\\n\"\n");
        return 1;
    }

    // Производим приведение типов к более широкому типу int, производим операцию в типе int,
    // Проверяем, что результат операции поместится в результирующий тип short. 
    int num1_ext = num1;
    int num2_ext = num2;
    int sum_ext = num1_ext + num2_ext;
    if (sum_ext < SHRT_MIN || sum_ext > SHRT_MAX)
    {
        printf("adder: %hd+%hd is too big to be represented in short\n", num1, num2);
        return 1;
    }

    // Производим сложение.
    short sum = sum_ext;

    // Выводим результат на экран.
    printf("%hd\n", sum);

    // Применяем альтернативный способ - встроенную функцию компилятора GCC.
    short rslt;
    bool error = __builtin_add_overflow(num1, num2, &rslt);
    if (error)
    {
        printf("adder: %hd+%hd is too big to be represented in short\n", num1, num2);
        return 1;
    }

    printf("%hd\n", rslt);

    return 0;
}
