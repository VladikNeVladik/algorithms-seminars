#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdint.h>

// Объявляем функции.
// Определения функций будут доступны после компоновки с macro.o.
int io_get_dec(void);
unsigned io_get_hex(void);

void io_print_dec(int v);
void io_print_hex(unsigned v);

void io_print_string(const char *s);

void io_newline(void);

static  uint8_t var_08bit;
static uint16_t var_16bit;
static uint32_t var_32bit;

static uint8_t buffer[10U] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static const uint32_t ushifted = 10U;
static const int32_t sshifted  = 10;

static uint32_t res0, res1;
static  int32_t res2, res3;

// Задание: изучите бинарный код этой функции при помощи дизассемблера.
void assembled_example_function()
{
    // Записи в переменные разного размера.
    var_08bit = 0x01U;
    var_16bit = 0x0001U;
    var_32bit = 0x00000001U;

    // Целочисленная арифметика.
    var_08bit += 1;
    var_16bit -= 1;
    var_32bit *= 10;
    var_32bit /= 5;

    // Вывод реузльтатов арифметических операций.
    io_print_hex(var_08bit);
    io_newline();
    io_print_hex(var_16bit);
    io_newline();
    io_print_hex(var_32bit);
    io_newline();

    // Доступы в массив.
    buffer[0] = 0xA;
    var_08bit = buffer[1];

    // Битовые сдвиги.
    res0 = (ushifted << 10U);
    res1 = (ushifted >> 10U);
    res2 = (sshifted << 10U);
    res3 = (sshifted >> 10U);

    // Сужающие преобразования типов.
    long long val64 = 1;
    unsigned  val32 = 2;
    short     val16 = 3;
    char      val08 = 4;

    val32 = (unsigned) val64;
    val16 = (short)    val32;
    val08 = (char)     val16;

    // Расширяющие преобразования типов.
    val64 = (long long) val32;
    val32 = (unsigned)  val16;
    val16 = (short)     val08;
}
