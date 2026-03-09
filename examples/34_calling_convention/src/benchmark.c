#ifndef __i386__
#error Wrong architecture!
#endif

#include <stdlib.h>
#include <stdint.h>

#define CALLING_CONVENTION __attribute__((cdecl))
// #define CALLING_CONVENTION __attribute__((stdcall))
// #define CALLING_CONVENTION __attribute__((fastcall))

typedef struct
{
    uint8_t array[16];
} Struct;

CALLING_CONVENTION uint32_t callingConvention(
    uint32_t arg1, uint32_t arg2, uint32_t arg3,
    uint32_t arg4, uint32_t arg5, uint32_t arg6)
{
    return arg1 + arg2 + arg3 + arg4 + arg5 + arg6;
}

CALLING_CONVENTION uint32_t addOne32(uint32_t arg)
{
    return arg + 1U;
}

CALLING_CONVENTION uint64_t addOne64(uint64_t arg)
{
    return arg + 1U;
}

CALLING_CONVENTION Struct addOneStruct(Struct arg)
{
    Struct ret = arg;

    arg.array[0] += 1;

    return ret;
}

int main()
{
    uint32_t rslt32 = callingConvention(1, 2, 3, 4, 5, 6);

    uint32_t  u32  = addOne32( 0U);
    uint64_t  u64  = addOne64( 0U);

    Struct structure = {
        .array = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
    };

    addOneStruct(structure);

    return EXIT_SUCCESS;
}