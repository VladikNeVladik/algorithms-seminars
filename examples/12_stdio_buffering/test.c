#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 2000000U

static char buffer1[BUFFER_SIZE];
static char buffer2[BUFFER_SIZE];
static char buffer3[BUFFER_SIZE];
static char buffer4[BUFFER_SIZE];
static char buffer5[BUFFER_SIZE];
static char buffer6[BUFFER_SIZE];
static char buffer7[BUFFER_SIZE];

static char buffer[BUFSIZ];

void benchmark_fgetc(char* rcv, FILE* file, int bufmode, const char* bufmode_set)
{
    memset(rcv, 0U, BUFFER_SIZE);
    
    setvbuf(file, buffer, bufmode, BUFSIZ);

    clock_t ticks_start = clock();
    for (size_t i = 0U; i < BUFFER_SIZE; ++i)
    {
        rcv[i] = fgetc(file);
    }
    clock_t ticks_end = clock();
    printf("Read  2MB with fgetc (%s):          %03lf ms\n",
        bufmode_set, (double) (ticks_end - ticks_start) * 1000U / CLOCKS_PER_SEC);
}

void benchmark_fread(char* rcv, FILE* file, int bufmode, const char* bufmode_set)
{
    memset(rcv, 0U, BUFFER_SIZE);

    setvbuf(file, buffer, bufmode, BUFSIZ);

    clock_t ticks_start = clock();
    size_t ret = fread(rcv, 1U, BUFFER_SIZE, file);
    (void) ret;
    clock_t ticks_end = clock();
    printf("Read  2MB in one read (%s):         %03lf ms\n",
        bufmode_set, (double) (ticks_end - ticks_start) * 1000U / CLOCKS_PER_SEC);
}

void benchmark_fgetc_unlocked(char* rcv, FILE* file, int bufmode, const char* bufmode_set)
{
    memset(rcv, 0U, BUFFER_SIZE);

    setvbuf(file, buffer, bufmode, BUFSIZ);

    flockfile(file);

    clock_t ticks_start = clock();
    for (size_t i = 0U; i < BUFFER_SIZE; ++i)
    {
        rcv[i] = fgetc_unlocked(file);
    }
    clock_t ticks_end = clock();
    printf("Read  2MB with unlocked fgetc (%s): %03lf ms\n",
        bufmode_set, (double) (ticks_end - ticks_start) * 1000U / CLOCKS_PER_SEC);

    funlockfile(file);
}

void benchmark_memset(char* rcv)
{
    memset(rcv, 0U, BUFFER_SIZE);

    clock_t ticks_start = clock();
    memset(rcv, 0U, BUFFER_SIZE);
    clock_t ticks_end = clock();
    printf("Write 2MB using memset:                         %03lf ms\n",
        (double) (ticks_end - ticks_start) * 1000U / CLOCKS_PER_SEC);
}

int main(void)
{
    FILE* file = fopen("/dev/zero", "r");
    assert(file != NULL);

    benchmark_fgetc(buffer1, file, _IOFBF, "full buffering");
    benchmark_fgetc(buffer2, file, _IONBF, "no buffering  ");

    benchmark_fgetc_unlocked(buffer3, file, _IOFBF, "full buffering");
    benchmark_fgetc_unlocked(buffer4, file, _IONBF, "no buffering  ");

    benchmark_fread(buffer5, file, _IOFBF, "full buffering");
    benchmark_fread(buffer6, file, _IONBF, "no buffering  ");

    benchmark_memset(buffer7);

    fclose(file);

    return EXIT_SUCCESS;
}
