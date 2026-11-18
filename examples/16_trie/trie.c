#include <stdlib.h>

#include "trie.h"

#define FLAGS_TYPE              0xC0
#define FLAGS_TYPE_LEAF         0x40
#define FLAGS_TYPE_BRANCH       0x80
#define FLAGS_TYPE_EXTENSION    0xC0

#define FLAGS_LENGTH 0x0F

#define INVALID_NODE 0xFFFFFFFFU

uint8_t nibble_get(uint8_t* nibbles, uint8_t index)
{
    uint8_t pos   = index/2U;
    uint8_t shift = (1U - (index & 1U)) * 4U;

    return (nibbles[pos] >> shift) & 0x0FU;
}

uint8_t nibble_set(uint8_t* nibbles, uint16_t index, uint8_t nibble)
{
    uint16_t pos   = index/2U;
    uint16_t shift = (1U - (index & 1U)) * 4U;

    nibbles[pos] &= ~(0x0FU << shift);
    nibbles[pos] |= (nibble << shift);
}

uint32_t trie_alloc_node(PATRICIA_TRIE* trie, uint32_t elementSize)
{
    if (trie->size + elementSize >= trie->capacity)
    {
        trie->capacity *= 2U;
        if (trie->capacity == 0U)
        {
            printf("dynamic_array_alloc: Reached reallocation limit\n");
            exit(EXIT_FAILURE);
        }

        // Пробуем выделить массив увеличенного размера.
        trie->array = realloc(
            trie->array,
            trie->capacity);
        if (trie->array == NULL)
        {
            printf("dynamic_array_alloc: Unable to reallocate array\n");
            exit(EXIT_FAILURE);
        }
    }

    // Индекс нового выделенного узла в массиве.
    uint32_t index = trie->size;

    // Увеличиваем кол-во выделенных элементов.
    trie->size = (trie->size + elementSize + 0x7) & ~0x7;

    return index;
}

void trie_init(PATRICIA_TRIE* trie)
{
    trie->array = (uint8_t) calloc(64U, 1U);
    if (trie->array == NULL)
    {
        printf("dynamic_array_init: Unable to allocate node\n");
        exit(EXIT_FAILURE);
    }

    trie->size = 0U;
    trie->capacity = 64U;

    trie->root = INVALID_NODE;
}

bool trie_search_helper(
    PATRICIA_TRIE* trie,
    uint8_t* nibbles,
    uint16_t size,
    uint16_t* currentNibble,
    uint32_t** prevNode,
    uint32_t* lastNode)
{
    if (trie->root == INVALID_NODE)
    { // Префиксное дерево пусто.
        return false;
    }

    // Признак завершения поиска.
    bool searchSuccessful = true;
    // Номер текущего полубайта.
    uint16_t current = 0U;
    // Следующий узел у предшественника lastNode.
    uint32_t* previous = &trie->root;
    // Индекс текущего узла.
    uint32_t nodeIndex = trie->root;

    while (current < size && searchSuccessful)
    {
        // Указатель на текущий узел;
        uint8_t* nodePtr = &trie->array[nodeIndex];

        // Определяем тип следующего узла в дереве.
        switch ((*nodePtr & FLAGS_TYPE))
        {
            case FLAGS_TYPE_LEAF:
            {
                LEAF_NODE* leaf = (LEAF_NODE*) nodePtr;
                // Кол-во полубайт в узле.
                uint8_t length = leaf->flags & FLAGS_LENGTH;

                // Номер полубайта в рамках узла.
                uint8_t i = 0;
                for (; i < length && current < size; ++i, ++current)
                {
                    uint8_t expectedNibble = nibble_get(leaf->nibbles, i);
                    uint8_t searchedNibble = nibble_get(nibbles, current);

                    if (expectedNibble != searchedNibble)
                    {   // Найден различающийся полубайт.
                        searchSuccessful = false;
                        break;
                    }
                }
                if (current == size && i < length)
                { // Узел содержит слишком мало полубайт.
                    searchSuccessful = false;
                }
                // Все полубайты искомого слова совпали.
                // ToDo: какая-то мешанина из случаев.

                break; 
            }
            case FLAGS_TYPE_BRANCH:
            {
                BRANCH_NODE* branch = (BRANCH_NODE*) nodePtr;

                break;
            }
            case FLAGS_TYPE_EXTENSION:
            {
                EXTENSION_NODE* extension = (EXTENSION_NODE*) nodePtr;

                break; 
            }
            default:
            {
                printf("ERROR: unexpected trie node type!");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Записываем возвращаемые значения.
    *prevNode = previous;
    *lastNode = nodeIndex;
    *currentNibble = current;
}

bool trie_insert(PATRICIA_TRIE* trie, uint8_t* nibbles, uint16_t size, uint64_t value)
{
    if (trie->root == INVALID_NODE)
    { // Префиксное дерево пусто.
        // Указатель на следующий узел для предыдущего узла.
        uint32_t* prevNext = &trie->root;

        // Признак "Листовой узел был записан".
        bool leafWritten = false;
        // Номер текущего полубайта.
        uint16_t current = 0U;
        // Формируем связный список из промежуточных и листовых узлов.
        while (!leafWritten)
        {
            // Количесто полубайт, которое осталось записать. 
            uint16_t nibblesLeft = size - current;

            if (nibblesLeft <= LEAF_NODE_MAX_NIBBLES)
            { // Оставшиеся полубайты поместятся в листовой узел.
                // Выделяем новый узел.
                uint32_t nodeIndex = trie_alloc_node(trie, sizeof(LEAF_NODE));
                // Устаналиваем указатель на следующий узел у предыдущего узла.
                *prevNext = nodeIndex;

                // Помечаем новый узел как листовой.
                LEAF_NODE* leaf = (LEAF_NODE*) &trie->array[nodeIndex];
                // Инициализируем поля листового узла.
                leaf->value = value;
                leaf->flags |= FLAGS_TYPE_LEAF;
                leaf->flags |= nibblesLeft & FLAGS_LENGTH;

                for (uint8_t i = 0U; i < nibblesLeft; ++i, ++current)
                {
                    uint8_t nibble = nibble_get(nibbles, current);
                    nibble_set(leaf->nibbles, i, nibble);
                }

                // Помечаем листовой узел как записанный.
                leafWritten = true;
            }
            else
            { // Оставшиеся полубайты не поместятся в листовой узел.
                // Выделяем новый узел.
                uint32_t nodeIndex = trie_alloc_node(trie, sizeof(EXTENSION_NODE));
                // Устаналиваем указатель на следующий узел у предыдущего узла.
                *prevNext = nodeIndex;

                // Помечаем новый узел как промежуточный.
                EXTENSION_NODE* extension =
                    (EXTENSION_NODE*) &trie->array[nodeIndex];

                // Кол-во полубайт, которое будет записано в промежуточный узел.
                uint16_t nibblesWritten =
                    (nibblesLeft <= EXTENSION_NODE_MAX_NIBBLES)? nibblesLeft : EXTENSION_NODE_MAX_NIBBLES;

                // Запрашиваем отложенную запись индекса следующего узла в массиве узлов.
                prevNext = &extension->next;
                // Инициализируем поля промежуточного узла.
                extension->flags |= FLAGS_TYPE_EXTENSION;
                extension->flags |= nibblesWritten & FLAGS_LENGTH;

                for (uint8_t i = 0U; i < nibblesWritten; ++i, ++current)
                {
                    uint8_t nibble = nibble_get(nibbles, current);
                    nibble_set(extension->nibbles, i, nibble);
                }
            }
        }
    }
    else
    { // Префиксное дерево непусто.

    }
}

bool trie_search(PATRICIA_TRIE* trie, uint8_t* nibbles, uint16_t size, uint32_t* value)
{
    
}