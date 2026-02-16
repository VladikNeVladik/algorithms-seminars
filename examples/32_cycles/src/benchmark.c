#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdint.h>

int x = 0, y = 0;

int array0[4][4];
int* array1[4];
int** array2;

// Задание: изучите бинарный код этой функции при помощи дизассемблера.
void example_arrays_vs_pointers(void)
{
    array0[x][y] = 1;

    array1[x][y] = 1;

    array2[x][y] = 1;
}
