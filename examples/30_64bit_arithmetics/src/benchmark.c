#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdint.h>

// Объявляем функции.
// Определения функций будут доступны после компоновки с macro.o.
int      io_get_dec(void);
unsigned io_get_hex(void);

void io_print_dec(int v);
void io_print_hex(unsigned v);

void io_print_string(const char *s);

void io_newline(void);

// Задание: изучите бинарный код этой функции при помощи дизассемблера.
void assembled_example_function(void)
{
    // Беззнаковые 32-битные числа.
    static uint32_t u32_a;
    static uint32_t u32_b;
    static uint32_t u32_c;
    static uint32_t u32_d;

    u32_c = u32_a * u32_b;
    u32_d = u32_a / u32_b;

    // Знаковые 32-битные числа.
    static int32_t s32_a;
    static int32_t s32_b;
    static int32_t s32_c;
    static int32_t s32_d;

    s32_c = s32_a * s32_b;
    s32_d = s32_a / s32_b;

    // Беззнаковые 64-битные числа.
    static uint64_t u64_a;
    static uint64_t u64_b;
    static uint64_t u64_c;
    static uint64_t u64_d;

    u64_c = u64_a * u64_b;
    u64_d = u64_a / u64_b;

    // Знаковые 64-битные числа.
    static int64_t s64_a;
    static int64_t s64_b;
    static int64_t s64_c;
    static int64_t s64_d;

    s64_c = s64_a * s64_b;
    s64_d = s64_a / s64_b;
}
