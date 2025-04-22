#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdint.h>

// Задание: изучите бинарный код этой функции при помощи дизассемблера.
void example_loop(void)
{
    static uint32_t arr[10U];

    uint32_t max = 0U;
    for (uint32_t i = 0; i < 10U; i++)
    {
        if (max < arr[i])
        {
            max = arr[i];
        }
    }
}
