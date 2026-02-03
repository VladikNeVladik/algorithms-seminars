#include <stdint.h>

int      io_get_dec(void);
unsigned io_get_hex(void);

void io_print_dec(int v);
void io_print_hex(unsigned v);

void io_print_string(const char *s);

void io_newline(void);

// Задание: изучите бинарный код этой функции при помощи дизассемблера.
void example_conditionals(void)
{
    static uint32_t u32_a, u32_b;
    static  int32_t s32_c, s32_d;

    // Пример #1.
    if (u32_a > u32_b)
    {
        return;
    }

    // Пример #2.
    if (u32_a != u32_b)
    {
        return;
    }

    // Пример #3.
    if (u32_a <= u32_b && s32_c >= s32_d)
    {
        return;
    }

    // Пример #4.
    if (s32_c == s32_d)
    {
        return;
    }

    // Пример #5.
    if (s32_c > s32_d)
    {
        return;
    }

    // Пример #6.
    if (s32_c >= s32_d || u32_a > u32_b)
    {
        return;
    }
}

typedef enum
{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_SIN,
    OP_COS,
    OP_EXP
} OperationType;

// Задание: изучите бинарный код этой функции при помощи дизассемблера.
void example_ifelse_chain(void)
{
    static OperationType op;
    static int result;

    if      (op == OP_ADD) { result = 0; }
    else if (op == OP_SUB) { result = 1; }
    else if (op == OP_MUL) { result = 2; }
    else if (op == OP_DIV) { result = 3; }
    else if (op == OP_SIN) { result = 4; }
    else if (op == OP_COS) { result = 5; }
    else if (op == OP_EXP) { result = 6; }
}

// Задание: изучите бинарный код этой функции при помощи дизассемблера.
void example_switch(void)
{
    static OperationType type;
    static int result;

    switch (type)
    {
    case OP_ADD:
        result = 0;
    case OP_SUB:
        result = 1;
        break;
    case OP_MUL:
        result = 2;
        break;
    case OP_DIV:
        result = 3;
        break;
    case OP_SIN:
        result = 4;
        break;
    case OP_COS:
        result = 5;
        break;
    case OP_EXP:
        result = 6;
        break;
    }
}
