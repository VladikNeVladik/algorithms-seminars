// Copyright 2024 Vladislav Aleinik
#include <string.h>

// Осторожно: API не из стандартной библиотеки (API POSIX)
#include <arpa/inet.h>

#include "database.h"
#include "utils.h"

// Кол-во элементов в базе данных
#define DB_ENTRIES 10U

// Бинарное представление IP-адреса
#define IP_ADDRESS(byte3, byte2, byte1, byte0)                  \
    ((((byte3) & 0xFFU) << 24U) | (((byte2) & 0xFFU) << 16U) |  \
     (((byte1) & 0xFFU) <<  8U) | (((byte0) & 0xFFU) <<  0U))

// Имя файла с базой данных
const char* DB_FILENAME = "res/database.db";

// Магические числа
// Используются для идентификации формата базы даных
const uint32_t MAGIC0 = 0xDEADBEEF;
const uint32_t MAGIC1 = 0xB01DFACE;

int main(void)
{
    // Массив строк для записи в БД
    const char* hostnames[DB_ENTRIES] =
    {
        "avacado",
        "potato",
        "grape",
        "lemon",
        "pineapple",
        "banana",
        "cucumber",
        "tomato",
        "melon",
        "orange"
    };

    // База данных для хранения ассоциативного массива
    struct Database db;

    // Выделяем память под базу даных
    db.entries = (Entry_t*) calloc(DB_ENTRIES, sizeof(Entry_t));
    verify_contract(db.entries != NULL, "Unable to allocate memory\n");

    db.num_entries = DB_ENTRIES;

    // Запись элементов в базу данных
    for (size_t db_i = 0; db_i < db.num_entries; ++db_i)
    {
        // Записываем IP-адрес
        uint32_t ip = IP_ADDRESS(192, 168, 0, db_i + 1);
        db.entries[db_i].key = htonl(ip);

        // Проверяем, что для записи строки согласно формату БД будет достаточно места
        verify_contract(
            strlen(hostnames[db_i]) <= sizeof(db.entries[db_i].value),
            "Hostname too long\n");

        // Записываем обладателя IP-адреса
        strcpy(db.entries[db_i].value, hostnames[db_i]);

        // Выводим на экран содержимое строки БД
        // printf("%-10s 192.168.0.%zu\n", hostnames[db_i], db_i + 1);
    }

    // Открываем файл на запись в бинарном виде
    FILE* file = fopen(DB_FILENAME, "wb+");
    verify_contract(file != NULL, "Unable to open file \'%s\'\n", DB_FILENAME);

    // Записываем два магических числа;
    uint32_t magics[2] = {
        htonl(MAGIC0),
        htonl(MAGIC1)
    };

    size_t ret = fwrite(magics, sizeof(MAGIC0), 2, file);
    verify_contract(ret == 2, "Unable to dump magic number to file \'%s\'\n", DB_FILENAME);

    // Записываем остальные данные
    ret = fwrite(db.entries, sizeof(Entry_t), DB_ENTRIES, file);
    verify_contract(ret == DB_ENTRIES, "Unable to dump magic number to file \'%s\'\n", DB_FILENAME);

    // Закрываем файл
    ret = fclose(file);
    verify_contract(ret != EOF,
        "Detected error on file close operation \'%s\'\n", DB_FILENAME);

    // Освобожаем память под базу данных
    free(db.entries);

    return EXIT_SUCCESS;
}
