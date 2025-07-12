// Copyright 2024 Vladislav Aleinik
#include <stdint.h>

typedef uint32_t Key_t;
typedef uint32_t Value_t;

#include "tree.h"

#define NUM_INSERTED      20U
#define NUM_FULL_SEARCHES 10U

int main(void)
{
    // Код возврата операции.
    RetCode ret;

    // Двоичное дерево для поиска элементов.
    Tree search_db;

    // Инициализируем двоичное дерево.
    ret = tree_alloc(&search_db);
    verify_contract(ret == RET_OK, "Unable to allocate tree\n");

    // Массив ключей для повтороного поиска в дереве.
    Key_t saved[NUM_INSERTED];

    // Задаём семя для генератора случайных чисел.
    srand(100500);

    for (size_t iteration_i = 0U; iteration_i < NUM_INSERTED; ++iteration_i)
    {
        // Пара ключ-значение для добавления в дерево.
        Key_t key = iteration_i;
        Value_t value = key;

        printf("===============\n");
        printf("TREE[%03d] = %03d\n", key, value);
        printf("===============\n\n");

        ret = tree_set(&search_db, key, value);
        verify_contract(ret == RET_OK, "Unable to insert tree element\n");

        if (iteration_i < NUM_INSERTED)
        {
            saved[iteration_i] = key;
        }
    }

    for (size_t search_i = 0U; search_i < NUM_FULL_SEARCHES; ++search_i)
    {
        for (size_t saved_i = 0U; saved_i < NUM_INSERTED; ++saved_i)
        {
            Key_t search_key = saved[saved_i];
            Value_t found_value = 0U;

            bool found;
            ret = tree_search(&search_db, search_key, &found_value, &found);
            verify_contract(ret == RET_OK, "Unable to search for tree element\n");
            verify_contract(found,
                "[TREE SEARCH] Unable to find an element\n");
            verify_contract(search_key == found_value,
                "[TREE SEARCH] Found unexpected value\n");
        }
    }

    for (size_t saved_i = 0U; saved_i < NUM_INSERTED; ++saved_i)
    {
        Key_t search_key = saved[saved_i];
        Value_t removed_value;

        printf("===========\n");
        printf("REMOVE[%03d]\n", search_key);
        printf("===========\n\n");

        bool removed;
        ret = tree_remove(&search_db, search_key, &removed_value, &removed);
        verify_contract(ret == RET_OK, "Unable to remove tree element\n");
        verify_contract(removed,
            "[TREE DELETION] Element not found\n");
        verify_contract(search_key == removed_value,
            "[TREE DELETION] Found unexpected value\n");
    }

    for (size_t saved_i = 0U; saved_i < NUM_INSERTED; ++saved_i)
    {
        Key_t search_key = saved[saved_i];
        Value_t found_value;

        bool found;
        ret = tree_search(&search_db, search_key, &found_value, &found);
        verify_contract(ret == RET_OK, "Unable to search for tree element\n");
        verify_contract(!found,
            "[TREE FAIL SEARCH] Found spurious element\n");
    }

    return EXIT_SUCCESS;
}
