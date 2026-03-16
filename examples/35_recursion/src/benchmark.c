#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdint.h>
#include <stdio.h>

unsigned isodd(unsigned n);

unsigned iseven(unsigned n)
{
    return (n == 0)? 1 : isodd(n - 1);
}

unsigned isodd(unsigned n)
{
    return (n == 0)? 0 : iseven(n - 1);
}

// NOTE: disassemble compiled executable of this function
void test(void)
{
    printf("%d\n", iseven(10));
}
