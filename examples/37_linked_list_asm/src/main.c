// Copyright 2026 Vladislav Aleinik
#include <queue.h>

#include <stdio.h>
#include <time.h>

#define NUM_ELEMENTS 200000000U

int main(void)
{
    // Начало измеряемого отрезка времени.
    clock_t start = clock();

    // Инициализируем очередь.
    Queue queue;
    queue_init(&queue);

    // Добавляем элементы в очередь.
    for (uint32_t i = 0U; i < NUM_ELEMENTS; ++i)
    {
        queue_add_tail(&queue, i);
    }

    // Прозводим обход связного списка.
    queue_scale_value(&queue, 2, 5);

    // Удаляем половину элементов из очереди.
    for (uint32_t i = 0U; i < NUM_ELEMENTS/2 && !queue_empty(&queue); ++i)
    {
        uint32_t elem;
        queue_remove_head(&queue, &elem);
    }
    
    // Прозводим повторный обход связного списка.
    queue_scale_value(&queue, 3, 7);

    // Освобождаем остальные элементы очереди.
    queue_free(&queue);

    // Конец измеряемого отрезка времени.
    clock_t end = clock();

    printf("Time: %lf\n", ((double) (end - start))/CLOCKS_PER_SEC);

    return 0;
}