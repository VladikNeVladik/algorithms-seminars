#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>

template <typename... Args>
void apply(int* array, size_t length, int (*fn) (const char*, ...), Args... args)
{
    for (size_t i = 0U; i < length; ++i)
    {
        fn(args..., array[i]);
    }
}

int main(void)
{
    // Выделяем память для вводимого массива.
    printf("Enter array size:\n");

    size_t array_size;
    scanf("%u", &array_size);

    int* array_ptr = (int*) calloc(array_size, sizeof(int));

    // Вводим данные.
    printf("Enter array elements:\n");

    for (size_t i = 0U; i < array_size; ++i)
    {
        scanf("%d", &array_ptr[i]);
    }

    // Вызываем функцию apply.
    apply(array_ptr, array_size, &printf, "%d\n");
}
