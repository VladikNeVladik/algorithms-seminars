// Copyright 2025 Vladislav Aleinik
#include <string.h>

#include "database.h"
#include "utils.h"

// Параметры теста на корректность.
#define NUM_INSERTED 10U
#define NUM_FULL_SEARCHES 10U

// Бинарное представление IP-адреса
#define IP_ADDRESS(byte3, byte2, byte1, byte0)                  \
    ((((byte3) & 0xFFU) << 24U) | (((byte2) & 0xFFU) << 16U) |  \
     (((byte1) & 0xFFU) <<  8U) | (((byte0) & 0xFFU) <<  0U))

// Имя файла с базой данных.
const char* DB_FILENAME = "res/database.db";

#define NUM_HOSTANAMES 10
const char* hostnames[NUM_HOSTANAMES] =
{
    "avacado",
    "potato",
    "grape",
    "lemon",
    "apple",
    "banana",
    "tomato",
    "melon",
    "orange",
    "cherry"
};

int main(void)
{
    //============================================//
    // Тест основной функциональности базы данных //
    //============================================//

    // База данных для поиска.
    struct Database db;
    db_alloc(&db);

    uint32_t saved[NUM_INSERTED];

    // Задаём семя для генератора случайных чисел.
    srand(100500);

    for (size_t iteration_i = 0U; iteration_i < NUM_INSERTED; ++iteration_i)
    {
        uint32_t key = rand();

        char value[VALUE_SIZE];
        snprintf(value, VALUE_SIZE, "%u", key);

        // Добавляем элемент в базу данных.
        db_insert(&db, key, value);

        saved[iteration_i] = key;
    }

    for (size_t search_i = 0U; search_i < NUM_FULL_SEARCHES; ++search_i)
    {
        for (size_t saved_i = 0U; saved_i < NUM_INSERTED; ++saved_i)
        {
            // Ключ для поиска в БД.
            uint32_t search_key = saved[saved_i];

            // Ожидаемое значение.
            char value_expected[VALUE_SIZE];
            snprintf(value_expected, VALUE_SIZE, "%u", search_key);

            // Производим поиск в БД.
            char value_found[VALUE_SIZE];
            uint32_t index;
            bool found = db_search(&db, search_key, value_found, &index);

            verify_contract(found,
                "[DB SEARCH] Unable to find an element\n");
            verify_contract(strcmp(value_expected, value_found) == 0,
                "[DB SEARCH] Found unexpected value\n");
        }
    }

    for (size_t saved_i = 0U; saved_i < NUM_INSERTED; ++saved_i)
    {
        // Ключ для поиска в БД.
        uint32_t search_key = saved[saved_i];

        // Ожидаемое значение.
        char value_expected[VALUE_SIZE];
        snprintf(value_expected, VALUE_SIZE, "%u", search_key);

        // Производим удаление элемента из БД.
        char value_removed[VALUE_SIZE];
        bool removed = db_remove(&db, search_key, value_removed);

        verify_contract(removed,
            "[DB DELETION] Element not found\n");
        verify_contract(strcmp(value_expected, value_removed) == 0,
            "[DB DELETION] Found unexpected value\n");
    }

    for (size_t saved_i = 0U; saved_i < NUM_INSERTED; ++saved_i)
    {
        // Ключ для поиска в БД.
        uint32_t search_key = saved[saved_i];

        // Производим поиск в БД.
        char value_found[VALUE_SIZE];
        uint32_t index;
        bool found = db_search(&db, search_key, value_found, &index);

        verify_contract(!found,
            "[DB FAIL SEARCH] Found spurious element\n");
    }

    // Освобождаем ресурсы базы данных.
    db_free(&db);

    //=======================//
    // Тест работы с файлами //
    //=======================//

    // База данных для сохранения в файл.
    struct Database db_write;
    db_alloc(&db_write);

    printf("Write to file:\n");

    // Запись элементов в базу данных.
    for (size_t db_i = 0; db_i < NUM_HOSTANAMES; ++db_i)
    {
        // Ключ в базе данных - IP-адрес.
        uint32_t ip = IP_ADDRESS(192, 168, 0, NUM_HOSTANAMES - db_i);

        // Проверяем, что для записи строки согласно формату БД будет достаточно места.
        verify_contract(
            strlen(hostnames[db_i]) < VALUE_SIZE,
            "Hostname too long\n");

        // Записываем обладателя IP-адреса в БД.
        char value[VALUE_SIZE] = {};
        strcpy(value, hostnames[db_i]);

        db_insert(&db_write, ip, value);

        // Выводим на экран содержимое строки БД
        printf("%-10s 192.168.0.%zu\n", hostnames[db_i], NUM_HOSTANAMES - db_i);
    }

    // Сохраняем базу данных в файл.
    db_dump_to_file(&db_write, DB_FILENAME);

    db_free(&db_write);

    // База данных для считывания из файла.
    struct Database db_read;
    db_scan_from_file(&db_read, DB_FILENAME);

    printf("Read from file:\n");

    for (uint32_t i = 0; i < db_size(&db_read); ++i)
    {
        uint32_t key;
        char value[VALUE_SIZE];

        db_at_index(&db_read, i, &key, value);

        // Записываем IP-адрес.
        uint8_t byte3 = (key >> 24U) & 0xFFU;
        uint8_t byte2 = (key >> 16U) & 0xFFU;
        uint8_t byte1 = (key >>  8U) & 0xFFU;
        uint8_t byte0 = (key >>  0U) & 0xFFU;

        // Выводим на экран содержимое строки БД
        printf("%-10s %hhu.%hhu.%hhu.%hhu\n", value, byte3, byte2, byte1, byte0);
    }

    return EXIT_SUCCESS;
}
