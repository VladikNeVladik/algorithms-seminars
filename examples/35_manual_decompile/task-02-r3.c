#include <stdio.h>

#define ARRAY_SIZE 21U

static int N;
static int K;

static int array_a[ARRAY_SIZE];
static int array_b[ARRAY_SIZE];

int main(void)
{
    scanf("%d", &N);
    scanf("%d", &K);

    int* a = array_a;
    int* b = array_b;

    a[0] = 1;
    int ecx = 0;
    while (ecx != N)
    {
        ecx++;

        b[0] = 1;
        int edx = 0;

        while (ecx != edx)
        {
            edx++;
            b[edx] = a[edx - 1] + a[edx];
        }

        b[edx] = 1;

        int* eax = a;
        a = b;
        b = eax;
    }

    printf("%d\n", a[K]);

    return 0;
}
