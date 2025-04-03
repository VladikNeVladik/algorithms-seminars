#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdio.h>

FILE *get_stdin(void) { return stdin; }
FILE *get_stdout(void) { return stdout; }


// Функции ввода-вывода, легко вызываемые из ASM:
// - Допускают стек, не выровненный по ABI.
// - Возвращаемое значение через EAX.
// - Передача до трёх аргументов через EAX, EDX, ECX.
// - Сохраняют неизменными EBX, EBP, ESP, EDI, ESI.
//
// См. macro.c из SASM (например, в папке /usr/share/sasm/NASM/macro.c)

#define IO_ATTR __attribute__((regparm(3),force_align_arg_pointer))

// Чтение 32-битного числа в формате %d/%u/%x, возврат значения через EAX
IO_ATTR int      io_get_dec(void);
IO_ATTR unsigned io_get_udec(void);
IO_ATTR unsigned io_get_hex(void);

// Чтение символа с помощью getchar(), возврат значения через EAX
IO_ATTR int io_get_char(void);

// Чтение строки в массив. ('buf' передаётся через EAX, 'size' через EDX)
IO_ATTR void io_get_string(char *buf, int size);

// Вывод 32-битного числа из EAX в формате %d/%u/%x.
IO_ATTR void io_print_dec(int v);
IO_ATTR void io_print_udec(unsigned v);
IO_ATTR void io_print_hex(unsigned v);

// Вывод символа из регистра AL
IO_ATTR void io_print_char(char c);

// Вывод строки, задаваемой по указателю EAX
IO_ATTR void io_print_string(const char *s);

// Перенос строки
IO_ATTR void io_newline(void);

IO_ATTR
int io_get_dec(void)
{
    int r;
    scanf("%d", &r);
    return r;
}

IO_ATTR
unsigned io_get_udec(void)
{
    unsigned r;
    scanf("%u", &r);
    return r;
}

IO_ATTR
unsigned io_get_hex(void)
{
    unsigned r;
    scanf("%X", &r);
    return r;
}

IO_ATTR
int io_get_char(void)
{
    return getchar();
}

IO_ATTR
void io_get_string(char *buf, int size)
{
    fgets(buf, size, stdin);
}

IO_ATTR
void io_print_dec(int v)
{
    printf("%d", v);
}

IO_ATTR
void io_print_udec(unsigned v)
{
    printf("%u", v);
}

IO_ATTR
void io_print_hex(unsigned v)
{
    printf("%X", v);
}

IO_ATTR
void io_print_char(char c)
{
    putchar(c);
}

IO_ATTR
void io_print_string(const char *s)
{
    fputs(s, stdout);
}

IO_ATTR
void io_newline(void)
{
    putchar('\n');
}

__attribute__((constructor))
static void unbuffer_stdout(void)
{
    setbuf(stdout, 0);
}
