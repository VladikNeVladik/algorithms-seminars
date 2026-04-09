// Copyright 2026 Vladislav Aleinik
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef int32_t Key_t;
typedef int32_t Value_t;
#include <tree-avl.h>

#include <utils.h>

int main(void)
{
    Tree tree;
    tree_alloc(&tree);

    // Управляющий символ.
    char command;
    int ret = scanf(" %c", &command);
    verify_contract(ret == 1, "Invalid input\n");

    while (command != 'F')
    {
        int32_t key, value;
        switch (command)
        {
            case 'A':
            {
                int ret = scanf("%d %d", &key, &value);
                verify_contract(ret == 2, "Command 'A': invalid arguments\n");

                tree_set(&tree, key, value);
                break;
            }
            case 'S':
            {
                int ret = scanf(" %d", &key);
                verify_contract(ret == 1, "Command 'S': invalid arguments\n");

                bool found;
                tree_search(&tree, key, &value, &found);

                if (found)
                {
                    printf("%d %d\n", key, value);
                }

                break;
            }
            case 'D':
            {
                int ret = scanf("%d", &key);
                verify_contract(ret == 1, "Command 'D': invalid arguments\n");

                bool found;
                tree_remove(&tree, key, &value, &found);

                break;
            }
            default:
            {
                printf("Unknown command\n");
                tree_free(&tree);
                return EXIT_FAILURE;
            }
        }

        int ret = scanf(" %c", &command);
        verify_contract(ret == 1, "Invalid input\n");
    }

    tree_free(&tree);

    return EXIT_SUCCESS;
}