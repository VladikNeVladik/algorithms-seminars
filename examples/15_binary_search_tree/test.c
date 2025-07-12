// Copyright 2024 Vladislav Aleinik
#include <stdint.h>
#include <unistd.h>

typedef uint32_t Key_t;
typedef uint32_t Value_t;

#include "tree.h"

#define NUM_INSERTED      30U
#define NUM_FULL_SEARCHES 10U

int main(void)
{
    Tree search_db;
    tree_alloc(&search_db);

    Key_t saved[NUM_INSERTED];

    // Задаём семя для генератора случайных чисел.
    srand(100500);

    for (size_t iteration_i = 0U; iteration_i < NUM_INSERTED; ++iteration_i)
    {
        Key_t key = rand();
        Value_t value = key;

        printf("=========================\n");
        printf("TREE[%08x] = %08x\n", key, value);
        printf("=========================\n");

        tree_set(&search_db, key, value);

        // Печатаем содержимое дерева.
        tree_print(&search_db);
        printf("\n");
        // sleep(1);

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
            tree_search(&search_db, search_key, &found_value, &found);
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

        printf("=========================\n");
        printf("REMOVE[%08x]\n", search_key);
        printf("=========================\n");

        // Печатаем содержимое дерева.
        tree_print(&search_db);
        printf("\n");
        // sleep(1);

        bool removed;
        tree_remove(&search_db, search_key, &removed_value, &removed);
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
        tree_search(&search_db, search_key, &found_value, &found);
        verify_contract(!found,
            "[TREE FAIL SEARCH] Found spurious element\n");
    }

    // Print the tree:
    tree_print(&search_db);

    return EXIT_SUCCESS;
}
