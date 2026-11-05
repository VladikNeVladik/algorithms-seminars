// Copyright 2024 Vladislav Aleinik

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void print_bits8(uint8_t value)
{
    for (int i = 0; i < 8; ++i)
    {
        putc(((value & 0x80) == 0)? '0' : '1', stdout);

        value <<= 1;
    }
}

char print_a(void)
{
    printf("Side effect \"A\"\n");

    return 'A';
}

char print_b(void)
{
    printf("Side effect \"B\"\n");

    return 'B';
}

int main(void)
{
    //====================//
    // Побитовые операции //
    //====================//

    // Используемые переменные.
    uint8_t a = 10;
    uint8_t b = 23;
    uint8_t c = 2;

    printf("a      = ");
    print_bits8(a);
    printf("\n");

    printf("b      = ");
    print_bits8(b);
    printf("\n");

    printf("c      = ");
    print_bits8(c);
    printf("\n");

    // Инверсия:
    printf("~a     = ");
    print_bits8(~a);
    printf("\n");

    // Побитовое ИЛИ (OR):
    printf("a  | b = ");
    print_bits8(a | b);
    printf("\n");

    // Побитов И (AND):
    printf("a  & b = ");
    print_bits8(a & b);
    printf("\n");

    // Побитовое исключающее ИЛИ (XOR):
    printf("a  ^ b = ");
    print_bits8(a ^ b);
    printf("\n");

    // Сдвиг битов влево:
    printf("a << c = ");
    print_bits8(a << c);
    printf("\n");

    // Сдвиг битов вправо:
    printf("a >> c = ");
    print_bits8(a >> c);
    printf("\n");

    printf("\n");

    //=======================//
    // Целочисленное деление //
    //=======================//

    printf("+8 / +3 = %2d, +8 %% +3 = %2d\n", +8 / +3, +8 % +3);
    printf("+8 / -3 = %2d, +8 %% -3 = %2d\n", +8 / -3, +8 % -3);
    printf("-8 / +3 = %2d, -8 %% +3 = %2d\n", -8 / +3, -8 % +3);
    printf("-8 / -3 = %2d, -8 %% -3 = %2d\n", -8 / -3, -8 % -3);

    printf("\n");

    //==================//
    // Побочные эффекты //
    //==================//

    printf("Conditional operator return value: %c\n", true? print_a() : print_b());

    int print_ret = print_a();
    print_ret = print_b();
    printf("Comma operator return value: %c\n", print_ret);

    return 0;
}
