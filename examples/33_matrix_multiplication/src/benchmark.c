#include <stdint.h>
#include <stdio.h>
#include <time.h>

// Как узнать размеры кешей:
// ❯ cat /sys/bus/cpu/devices/cpu0/cache/index0/size
// 32K
// ❯ cat /sys/bus/cpu/devices/cpu0/cache/index1/size
// 32K
// ❯ cat /sys/bus/cpu/devices/cpu0/cache/index2/size
// 256K
// ❯ cat /sys/bus/cpu/devices/cpu0/cache/index3/size
// 6144K

// Размер L3-кеша.
#define L3_CACHE_SIZE (6U*1024U*1024U)

// Размер матрицы.
#define N 1024

static uint32_t A[N][N];
static uint32_t B[N][N];
static uint32_t C[N][N];
char resetter[L3_CACHE_SIZE];

void reset_cache()
{
    for (size_t i = 0; i < L3_CACHE_SIZE; i++)
    {
        resetter[i] = 'm';
    }
}

void matrix_init(uint32_t mat[N][N])
{
    size_t i_s = 1;
    for (size_t i = 0; i < N; i++ )
    {
        i_s += 1;
        size_t j_s = 1 + i_s;
        for (size_t j = 0; j < N; j++ )
        {
            j_s += 2;
            mat[i][j] = j_s;
        }
    }
}

void matrix_zero(uint32_t mat[N][N])
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            mat[i][j] = 0;
        }
    }
}

void matrix_transpose(uint32_t mat[N][N])
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            uint32_t d = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = d;
        }
    }
}

uint32_t matrix_verify(uint32_t mat[N][N])
{
    uint32_t sum = 0.0;
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            sum += mat[i][j];
        }
    }

    return sum;
}

// Сравнение производительности обычного перемножения матриц и перемножения с транспонированием.
void compare_matrix_multiplication_time(void)
{
    matrix_init(A);
    matrix_init(B);
    matrix_zero(C);
    reset_cache();

    // Начало измеряемого отрезка времени.
    clock_t ticks_start = clock();

    // Обычное переменожение матриц.
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            for (size_t k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Конец измеряемого отрезка времени
    clock_t ticks_end = clock();

    double delta = (ticks_end - ticks_start);
    double seconds = delta / CLOCKS_PER_SEC;
    uint32_t checksum = matrix_verify(C);
    printf("Обычное произведение матриц:             %7.2lf (контрольная сумма - %u)\n", seconds, checksum);

    matrix_init(A);
    matrix_init(B);
    matrix_zero(C);
    reset_cache();

    // Начало измеряемого отрезка времени.
    ticks_start = clock();

    // Переменожение матриц с транспонированием.
    matrix_transpose(B);
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            for (size_t k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[j][k];
            }
        }
    }
    matrix_transpose(B);

    // Конец измеряемого отрезка времени
    ticks_end = clock();

    delta = (ticks_end - ticks_start);
    seconds = delta / CLOCKS_PER_SEC;
    checksum = matrix_verify(C);
    printf("Произведение матриц с транспонированием: %7.2lf (контрольная сумма - %u)\n", seconds, checksum);
}
