#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdint.h>
#include <stdio.h>

// Expect program to be linked with definition of macro functions:
int      io_get_dec(void);
unsigned io_get_hex(void);

void io_print_dec(int v);
void io_print_hex(unsigned v);

void io_print_string(const char *s);

void io_newline(void);

// NOTE: disassemble compiled executable of this function
void example_helloworld(void)
{
    printf("Hello, world!\n");
}
