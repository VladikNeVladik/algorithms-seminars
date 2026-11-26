// Copyright 2026 Vladislav Aleinik
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include <time.h>

// Цвета для вывода в консоль.
#define COLOR_BYELLOW "\033[1;33m"
#define COLOR_BCYAN   "\033[1;36m"
#define COLOR_RESET   "\033[0m"

// Проверяет, что массив отсортирован.
bool is_sorted(int* array, uint32_t size)
{
    for (uint32_t i = 0U; i + 1U < size; ++i)
    {
        if (array[i] > array[i+1U])
        {
            return false;
        }
    }

    return true;
}

// Меняет местами два числа по указателям.
void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Возвращает индекс минимального элемента в непустом массиве.
uint32_t min_element(int* array, uint32_t size)
{
    uint32_t minIndex = 0U;
    int min = array[0U];
    for (uint32_t i = 1U; i < size; ++i)
    {
        if (array[i] < min)
        {
            min = array[i];
            minIndex = i;
        }
    }

    return minIndex;
}

// Возвращает верхнюю грань для числового массива.
// Верхняя грань - это такое число ub, что:
// - 0 <= ub <= size
// - Если 0 <= k < ub, то array[k] <= value
// - Если ub <= k < size, то value < array[k]
uint32_t upper_bound(int* array, uint32_t size, int value)
{
    uint32_t left  = 0U;
    uint32_t right = size;

    while (left < right)
    {
        uint32_t middle = left + (right - left)/2U;

        if (array[middle] <= value)
        {
            left = middle + 1U;
        }
        else
        {
            right = middle;
        }
    }

    return right;
}

// Производит поворот значений в непустом массиве вправо на 1 позицию.
void rotate_right_by_one(int* array, uint32_t size)
{
    int tmp = array[size - 1U];

    for (uint32_t i = size - 1U; i != 0U; --i)
    {
        array[i] = array[i - 1U];
    }

    array[0U] = tmp;
}

// Пузырьковая сортировка массива.
void bubble_sort(int* array, uint32_t size)
{
    if (size == 0)
    {
        return;
    }

    // Сравниваем все пары чисел в массиве.
    for (uint32_t i = 1U; i < size; ++i)
    {
        for (uint32_t j = 0U; j < size - i; ++j)
        {
            if (array[j] > array[j + 1U])
            {   // Для пары индексов (i,j) имеется инверсия значений.
                // Убираем эту инверсию.
                swap(&array[j], &array[j + 1U]);
            }
        }
    }
}

// Сортировка выбором.
void selection_sort(int* array, uint32_t size)
{
    for (uint32_t i = 0U; i < size; ++i)
    {
        uint32_t minIndex = i + min_element(&array[i], size - i);

        if (i < minIndex)
        {
            swap(&array[i], &array[minIndex]);
        }
    }
}

// Сортировка вставками.
void insertion_sort(int* array, uint32_t size)
{
    for (uint32_t i = 0U; i < size; ++i)
    {
        uint32_t insertPosition = upper_bound(array, i, array[i]);

        rotate_right_by_one(
            &array[insertPosition],
            i - insertPosition + 1U);
    }
}

int quicksort_comparator(const void* valA, const void* valB)
{
    // Преобразуем указатели к нужному типу.
    int* ptrA = (int*) valA;
    int* ptrB = (int*) valB;

    return *ptrA - *ptrB;
}

// Бистрая сортировка из стандартной библиотеки языка C.
void librarysort(int* array, uint32_t size)
{
    qsort(array, size, sizeof(int), &quicksort_comparator);
}

//===================//
// Алгоритм HeapSort //
//===================//

// Возвращает индекс родительского узла для узла index.
uint32_t parent(uint32_t index)
{
    return (index + 1)/2 - 1;
}

// Возвращает индекс левого дочернего узла для узла index.
uint32_t left(uint32_t index)
{
    return 2*index + 1;
}

// Возвращает индекс правого дочернего узла для узла index.
uint32_t right(uint32_t index)
{
    return 2*index + 2;
}

// Переносит узел из корня кучи в её лист,
// восстанавливая главное свойство Max-кучи.
void max_heapify(int* array, uint32_t heapSize, uint32_t index)
{
    // Индекс левого дочернего узла для узла index.
    uint32_t leftIndex = left(index);
    // Индекс правого дочернего узла для узла index.
    uint32_t rightIndex = right(index);

    // Индекс наибольшего из дочерних узлов для узла index.
    uint32_t largest = index;
    if (leftIndex < heapSize && array[leftIndex] > array[index])
    {
        largest = leftIndex;
    }
    if (rightIndex < heapSize && array[rightIndex] > array[largest])
    {
        largest = rightIndex;
    }

    if (largest != index)
    {
        // Восстанвливаем совйство Max-кучи локально для индекса index.
        swap(&array[index], &array[largest]);
        // Восстанавливаем свойство Max-кучи в оставшейся части структуры данных.
        max_heapify(array, heapSize, largest);
    }
}

// Превращает массив в Max-кучу.
void build_max_heap(int* array, uint32_t heapSize)
{
    for (uint32_t index = heapSize/2U; index >= 1U; --index)
    {
        max_heapify(array, heapSize, index - 1U);
    }
}

void heapsort(int* array, uint32_t size)
{
    // Строим Max-кучу из всего массива.
    build_max_heap(array, size);

    // Превращаем массив в Max-кучу, при этом поддерживая
    // Max-кучу в начале массива.
    for (uint32_t index = size - 1U; index >= 1U; --index)
    {
        swap(&array[0], &array[index]);
        max_heapify(array, index, 0U);
    }
}

//====================//
// Алгоритм QuickSort //
//====================//

uint32_t partition(int* array, uint32_t from, uint32_t to)
{
    int pivot = array[to];

    uint32_t leftIndex = from;
    for (uint32_t rightIndex = from; rightIndex <= to - 1U; ++rightIndex)
    {
        if (array[rightIndex] <= pivot)
        {
            swap(&array[leftIndex], &array[rightIndex]);
            leftIndex++;
        }
    }

    swap(&array[leftIndex], &array[to]);
    return leftIndex;
}

void quicksort_partial(int* array, uint32_t from, uint32_t to)
{
    if (from >= to || to == UINT32_MAX)
    {   // Сортировка не требуется.
        return;
    }

    uint32_t pivot = partition(array, from, to);

    quicksort_partial(array, from, pivot - 1U);
    quicksort_partial(array, pivot + 1U, to);
}

void quicksort(int* array, uint32_t size)
{
    quicksort_partial(array, 0U, size - 1U);
}

//=============================//
// Главная процедура программы //
//=============================//

#define MAX_ARRAY_SIZE   50000U
#define ARRAY_SIZE_START  2000U
#define ARRAY_SIZE_STEP   2000U

static int randomData[MAX_ARRAY_SIZE];
static int sortedArray[MAX_ARRAY_SIZE];

int main(void)
{
    // Инициализируем массив случайными данными.
    for (uint32_t i = 0U; i < MAX_ARRAY_SIZE; ++i)
    {
        randomData[i] = rand();   
    }

    //---------------------------------//
    // Сравнение алгоритмов сортировки //
    //---------------------------------//

    // Функции алгоритмов поиска подстроки
    void (*algorithms[])(int*, uint32_t) =
    {
        &heapsort,
        &quicksort,
        &librarysort,
        &insertion_sort,
        &selection_sort,
        &bubble_sort
    };

    // Названия алгоритмов сортировки.
    const char* algorithmNames[] =
    {
        "HeapSort (пирамидальная сортировка)",
        "QuickSort (быстрая сортировка)",
        "Библиотечная реализация qsort",
        "Сортировка вставками",
        "Сортировка выбором",
        "Пузырьковая сортировка"
    };

    for (unsigned algI = 0; algI < 4; ++algI)
    {
        printf(COLOR_BCYAN "%.60s\n" COLOR_RESET, algorithmNames[algI]);

        for (uint32_t arraySize = ARRAY_SIZE_START; arraySize <= MAX_ARRAY_SIZE; arraySize += ARRAY_SIZE_STEP)
        {
            // Копируем случайные данные в сортируемый массив.
            memcpy(sortedArray, randomData, arraySize * sizeof(int));

            // Начало измеряемого отрезка времени.
            clock_t ticksStart = clock();

            // Сортируем массив.
            algorithms[algI](sortedArray, arraySize);

            // Конец измеряемого отрезка времени
            clock_t ticksEnd = clock();

            if (!is_sorted(sortedArray, arraySize))
            {
                printf("Unable to sort array!\n");
                exit(EXIT_FAILURE);
            }

            double ticksDelta = ticksEnd - ticksStart;
            double seconds = ticksDelta / CLOCKS_PER_SEC;

            (void) seconds;
            printf(COLOR_BYELLOW "%7d, %10.3lfs\n" COLOR_RESET, arraySize, seconds);
        }
    }

    return EXIT_SUCCESS;
}
