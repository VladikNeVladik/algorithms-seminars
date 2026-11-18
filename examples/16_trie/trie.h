// Copyright 2026 Vladislav Aleinik
#ifndef HEADER_GUARD_TRIE_H_INCLUDED
#define HEADER_GUARD_TRIE_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

#define EXTENSION_NODE_MAX_NIBBLES 14U
#define LEAF_NODE_MAX_NIBBLES 14U

typedef struct
{
    // Поле, содержащее:
    // - Динамический тип узла.
    // - Кол-во полубайт.
    uint8_t flags;
    // От 0 до 14 полубайт.
    uint8_t nibbles[EXTENSION_NODE_MAX_NIBBLES/2U];
    // Индекс следующего узла в массиве узлов.
    uint32_t next;
} EXTENSION_NODE;

typedef struct
{
    // Поле, содержащее динамический тип узла.
    uint8_t flags;
    uint8_t padding[3];
    // Индекс следующего узла в массиве.
    uint32_t next[16];
} BRANCH_NODE;

typedef struct
{
    // Поле, содержащее:
    // - Динамический тип узла.
    // - Кол-во полубайт.
    uint8_t flags;
    // От 0 до 14 полубайт.
    uint8_t nibbles[LEAF_NODE_MAX_NIBBLES/2U];
    uint64_t value;
} LEAF_NODE;

typedef struct
{
    // Массив для выделения узлов.
    uint8_t* array;
    // Ёмкость массива для выделения узлов.
    uint32_t capacity;
    // Кол-во выделенных байт.
    uint32_t size;

    uint32_t root;
} PATRICIA_TRIE;

void trie_init(PATRICIA_TRIE* trie);
bool trie_insert(PATRICIA_TRIE* trie, uint8_t* nibbles, uint16_t size, uint64_t value);
bool trie_search(PATRICIA_TRIE* trie, uint8_t* nibbles, uint16_t size, uint64_t* value);

#endif // HEADER_GUARD_TRIE_H_INCLUDED
