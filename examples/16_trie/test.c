// Copyright 2024 Vladislav Aleinik
#include <stdint.h>
#include <stdio.h>

#include "trie.h"
#include "utils.h"

#define STRING_SIZE 100U

int main(void)
{
	// Инициализируем префиксном дереве.
	PATRICIA_TRIE trie;
	trie_init(&trie);

	char input[STRING_SIZE + 1U];
	for (uint64_t i = 0U; true; ++i)
	{
		printf("Enter a string №%lu:\n", i);

		// Считываем новую строку.
		int len = 0U;
		int ret = scanf("%100s%n", input, &len);
		verify_contract(ret == 1, "ERROR: expected a string\n");
		input[len] = '\0';

		// Производим поиск строки в префиксном дереве.
		uint64_t value;
		bool found = trie_search(&trie, input, len * 2U, &value);
		if (found)
		{
			printf("FOUND %lu\n", value);
		}
		else
		{
			printf("INSERT \"%s\"\n", input);
			bool inserted = trie_insert(&trie, input, len * 2U, i);
			verify_contract(inserted, "ERROR: unable to insert trie node\n");
		}
	}
}
