// Copyright 2026 Vladislav Aleinik
#include <queue.h>

#include <stdlib.h>

void queue_init(Queue* queue)
{
    // Корневой узел кольцевого двусвязного списка.
    Node* root = &queue->root;

    // Результат инициализации - пустая очередь.
    root->next = root;
    root->prev = root;
}

void queue_free(Queue* queue)
{
    // Корневой узел кольцевого двусвязного списка.
    Node* root = &queue->root;

    // Первый узел в очереди
    Node* node = root->next;

    // Освобождаем память всех узлов связного списка.
    while (node != root)
    {
        // Следующий узел связного списка.
        Node* next = node->next;

        // Освобождаем память текущего узла.
        free(node);

        // Переходим к следующему узлу.
        node = next;
    }
}

//====================================//
// Доступ к элементам связного списка //
//====================================//

int queue_add_tail(Queue* queue, const uint32_t data)
{
    // Выделяем память нового узла
    Node* new = malloc(sizeof(Node));
    if (new == NULL)
    {
        return -1;
    }

    // Корневой узел кольцевого двусвязного списка.
    Node* root = &queue->root;

    // Хвостовой узел кольцевого двусвязного списка.
    Node* tail = root->prev;

    // Связываем новый узел с хвостовым и корневым узлами.
    new->next = root;
    new->prev = tail;

    // Связываем корневой и хвостовой узлы с новым узлом.
    root->prev = new;
    tail->next = new;

    // Записываем данные нового узла.
    new->value = data;

    return 0;
}

int queue_remove_head(Queue* queue, uint32_t* data)
{
    // Корневой узел кольцевого двусвязного списка.
    Node* root = &queue->root;

    // Головной узел кольцевого двусвязного списка.
    Node* head = root->next;

    if (head == root)
    {   // Очередь пуста.
        // Удаление элемента из пустой очереди невозможно.
        return -1;
    }

    // Удаляем голову из списка.
    root->next = head->next;
    head->next->prev = root;

    // Сохраняем данные из головы списка.
    *data = head->value;

    // Освобождаем память головного узла.
    free(head);

    return 0;
}

bool queue_empty(Queue* queue)
{
    // Корневой узел кольцевого двусвязного списка.
    Node* root = &queue->root;

    // Головной узел кольцевого двусвязного списка.
    Node* head = root->next;

    // Признак пустоты очереди.
    return head == root;
}

void queue_scale_value(Queue* queue, uint32_t mul, uint32_t add)
{
    // Корневой узел кольцевого двусвязного списка.
    Node* root = &queue->root;

    for (Node* node = root->next; node != root; node = node->next)
    {
        node->value = node->value * mul + add; 
    } 
}