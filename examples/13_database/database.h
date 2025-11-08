// Copyright 2025 Vladislav Aleinik
#include <stdint.h>
#include <stdint.h>

#include "utils.h"

//==================//
// Структура данных //
//==================//

// Размер значения в базе данных.
#define VALUE_SIZE 12U

// Пара ключ-значение в базе данных.
typedef struct {
    // По ключу производится поиск в базе данных.
    uint32_t key;

    // Значение - то, что непосредственно хранится в базе данных.
    char value[VALUE_SIZE];
} Entry_t;

// Генерируем ошибку сборки при наличии дыр в структуре.
STATIC_ASSERT(sizeof(Entry_t) == 16U, entry_t_size);

// Представление ассоциативной поисковой структуры данных
struct Database
{
    // Пары ключ-значение, содержащиеся в базе данных.
    Entry_t* entries;
    // Количество элементов в базе данных.
    uint32_t size;
    // Размер массива.
    uint32_t capacity;
};

//======================//
// Управление ресурсами //
//======================//

//==================================================================================================
// Функция: db_alloc
// Назначение: Инициализирует базу данных.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db (out) - указатель на базу данных.
//
// Возвращаемое значение:
// Отсутствует
//==================================================================================================
void db_alloc(struct Database* db)
{
    // Выделяем память под базу данных минимального размера.
    db->entries = calloc(1U, sizeof(Entry_t));
    verify_contract(db->entries != NULL,
        "db_alloc: unable to allocate memory\n");

    // Устанавливаем размер и ёмкость.
    db->size     = 0;
    db->capacity = 1;
}

//==================================================================================================
// Функция: db_free
// Назначение: Освобождает ресурсы под базу данных.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db (in) - указатель на базу данных.
//
// Возвращаемое значение:
// Отсутствует
//==================================================================================================
void db_free(struct Database* db)
{
    free(db->entries);

    // Записываем мусорные значения в переменные.
    db->entries  = NULL;
    db->size     = 0;
    db->capacity = 0;
}

//===============================//
// Доступ к элементам по индексу //
//===============================//

//==================================================================================================
// Функция: db_size
// Назначение: Возвращает количество элементов в базе данных.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db (in) - указатель на базу данных.
//
// Возвращаемое значение:
// Количество элементов в базе данных.
//==================================================================================================
uint32_t db_size(const struct Database* db)
{
    return db->size;
}

//==================================================================================================
// Функция: db_at_index
// Назначение: Считывает ключ и значение по индексу в базе данных.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db    (in)  - указатель на базу данных.
// index (in)  - индекс элемента а базе данных.
// key   (out) - указатель на записываемый функцию ключ.
// value (out) - указатель на записываемое функцией значение.
//
// Возвращаемое значение:
// Отсутствует.
//
// Примечания:
// - Вызов функций db_insert и db_remove могут менять ключ-значение по индексу. 
//==================================================================================================
void db_at_index(const struct Database* db, uint32_t index, uint32_t* key, char value[VALUE_SIZE])
{
    verify_contract(index < db->size,
        "db_at_index: unable to get element at index %u (size is %u)\n",
        index, db->size);

    *key = db->entries[index].key;
    memcpy(value, db->entries[index].value, VALUE_SIZE);
}

//================================//
// Доступ к элементам БД по ключу //
//================================//

//==================================================================================================
// Функция: db_search
// Назначение: Ищет в базе данных значение по заданному ключу.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db    (in) - указатель на базу данных.
// key   (in) - искомый ключ.
// value (in) - указатель на записываемое функцией значение.
// index (in) - указатель на индекс элемента а базе данных.
//
// Возвращаемое значение:
// TRUE  - значение по ключу найдено.
// FALSE - значение по ключу отсутствует. 
//
// Примечания:
// - Вызов функций db_insert и db_remove могут менять ключ-значение по индексу. 
//==================================================================================================
bool db_search(const struct Database* db, uint32_t key, char value[VALUE_SIZE], uint32_t* index)
{
    if (db->size == 0)
    {
        return false;
    }

    // Индексы в массиве.
    uint32_t low  = 0;
    uint32_t high = db->size;

    // Производим бинарный поиск в отсортированном массиве.
    while (low < high)
    {
        uint32_t mid = low + (high - low)/2;

        if (db->entries[mid].key > key)
        {
            high = mid;
        }
        else if (db->entries[mid].key < key)
        {
            low = mid + 1;
        }
        else
        {
            memcpy(value, db->entries[mid].value, VALUE_SIZE);
            *index = mid;
            return true;
        }
    }

    // Ключ отсутствует в массиве. 
    return false;
}

//==================================================================================================
// Функция: db_insert
// Назначение: Вставляет в базу данных значение по заданному ключу.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db    (in) - указатель на базу данных.
// key   (in) - ключ для вставки в базу данных.
// value (in) - значение для вставки в базу данных.
//
// Возвращаемое значение:
// FALSE - элемент уже присутствовал в базе данных, значение обновлено.
// TRUE  - элемент успешно добавлен в базу данных.
//==================================================================================================
bool db_insert(struct Database* db, uint32_t key, const char value[VALUE_SIZE])
{
    if (db->size == db->capacity)
    {
        // Двукратно увеличивем объём выделенной памяти.
        db->capacity *= 2U;

        db->entries = realloc(db->entries, db->capacity * sizeof(Entry_t));
        verify_contract(db->entries != NULL, "db_insert: unable to reallocate memory\n");
    }

    // Индексы в базе данных.
    uint32_t low  = 0;
    uint32_t high = db->size; 

    // Находим место для вставки элемента.
    while (low < high)
    {
        uint32_t mid = low + (high - low)/2;

        if (db->entries[mid].key <= key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    // Место для вставки элемента.
    uint32_t pos = high;

    // Обновляем значение для заданного ключа. 
    if (pos != 0 && db->entries[pos - 1].key == key)
    {
        memcpy(db->entries[pos - 1].value, value, VALUE_SIZE);
        return false;
    }

    if (pos != db->size)
    {
        // Сдвигаем элементы на один вправо.
        memmove(&db->entries[pos + 1], &db->entries[pos], (db->size - pos)  * sizeof(Entry_t));
    }

    // Производим вставку элемента.
    db->entries[pos].key = key;
    memcpy(db->entries[pos].value, value, VALUE_SIZE);
    db->size++;
    return true;
}

//==================================================================================================
// Функция: db_remove
// Назначение: Удаляет из базы данных значение по заданному ключу.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db    (in)  - указатель на базу данных.
// key   (in)  - ключ для поиска в базе данных.
// value (out) - значение, которое удалено из базы данных.
//
// Возвращаемое значение:
// FALSE - элемент уже отсутствует в базе данных.
// TRUE  - элемент успешно удалён из базы данных.
//==================================================================================================
bool db_remove(struct Database* db, uint32_t key, char value[VALUE_SIZE])
{
    // Индекс элемента для удаления.
    uint32_t removeIndex;

    // Производим поиск элемента в массиве.
    bool found = db_search(db, key, value, &removeIndex);
    if (!found)
    {
        return false;
    }

    // Сдвигаем элементы на один влево.
    if (removeIndex != db->size - 1)
    {
        memmove(
            &db->entries[removeIndex],
            &db->entries[removeIndex + 1],
            (db->size - (removeIndex + 1))  * sizeof(Entry_t));
    }

    db->size--;

    // Освобождаем память, если это необходимо.
    if (4 <= db->capacity && db->size <= db->capacity/4)
    {
        db->capacity /= 2;

        db->entries = realloc(db->entries, db->capacity * sizeof(Entry_t));
        verify_contract(db->entries != NULL,
            "db_remove: unable to reallocate memory\n");
    }

    return true;
}

//=================================//
// Чтение из файла и запись в файл //
//=================================//

// Магические числа.
// Используются для идентификации формата базы даных
const uint32_t MAGIC0 = 0xDEADBEEF;
const uint32_t MAGIC1 = 0xB01DFACE;

//==================================================================================================
// Функция: db_dump_to_file
// Назначение: Сохраняет базу данных в файл.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db       (in) - указатель на базу данных.
// filename (in) - имя файла для сохранения данных.
//
// Возвращаемое значение:
// Отсутствует.
//==================================================================================================
void db_dump_to_file(const struct Database* db, const char* filename)
{
    // Открываем файл на запись в бинарном виде
    FILE* file = fopen(filename, "wb+");
    verify_contract(file != NULL,
        "db_dump_to_file: Unable to open file \'%s\'\n", filename);

    // Записываем два магических числа в формате Big Endian.
    uint32_t magics[2] = {
        my_htobe32(MAGIC0),
        my_htobe32(MAGIC1)
    };

    size_t ret = fwrite(magics, sizeof(MAGIC0), 2, file);
    verify_contract(ret == 2,
        "db_dump_to_file: Unable to dump magic number to file \'%s\'\n", filename);

    // Записываем остальные данные.
    for (uint32_t i = 0U; i < db->size; ++i)
    {
        uint32_t key = my_htobe32(db->entries[i].key);

        ret = fwrite(&key, sizeof(uint32_t), 1U, file);
        verify_contract(ret == 1,
            "db_dump_to_file: Unable to dump data to file \'%s\'\n", filename);

        ret = fwrite(db->entries[i].value, 1U, VALUE_SIZE, file);
        verify_contract(ret == VALUE_SIZE,
            "db_dump_to_file: Unable to dump data to file \'%s\'\n", filename);
    }

    // Закрываем файл.
    ret = fclose(file);
    verify_contract(ret != EOF,
        "db_dump_to_file: Detected error on file close operation \'%s\'\n", filename);
}

//==================================================================================================
// Функция: db_scan_from_file
// Назначение: Считывает базу данных из файла.
//--------------------------------------------------------------------------------------------------
// Параметры:
// db       (in) - указатель на базу данных.
// filename (in) - имя файла для чтения данных.
//
// Возвращаемое значение:
// Отсутствует.
//==================================================================================================
void db_scan_from_file(struct Database* db, const char* filename)
{
    // Открываем файл на чтение в бинарном виде
    FILE* file = fopen(filename, "rb+");
    verify_contract(file != NULL,
        "Unable to open file \'%s\'\n", filename);

    // Измеряем размер файла.
    int ret = fseek(file, 0L, SEEK_END);
    verify_contract(
        ret != -1,
        "Unable to find end of file: %s\n", filename
    );

    long fileSize = ftell(file);
    verify_contract(
        fileSize != -1,
        "Unable to measure size for file: %s\n", filename
    );

    // Возвращаем каретку на исходную точку.
    ret = fseek(file, 0L, SEEK_SET);
    verify_contract(
        ret != -1,
        "Unable to rewind to start of file: %s\n", filename
    );

    // Считываем и проверяем магические числа.
    uint32_t magics[2];

    size_t retf = fread(magics, sizeof(MAGIC0), 2, file);
    verify_contract(retf == 2,
        "Unable to read magic number from file \'%s\'\n",
        filename);
    verify_contract(
        my_be32toh(magics[0]) == MAGIC0 && my_be32toh(magics[1]) == MAGIC1,
        "Invalid magic numbers\n");

    // Вычисляем количество элементов в базе данных.
    db->size = (fileSize - sizeof(MAGIC0) - sizeof(MAGIC1)) / sizeof(Entry_t);
    db->capacity = db->size;

    // Выделяем массив для хранения элементов базы данных.
    db->entries = calloc(db->size, sizeof(Entry_t));
    verify_contract(db->entries != NULL,
        "Unable to allocate memory\n");

    // Считываем элементы базы данных
    retf = fread(db->entries, sizeof(Entry_t), db->size, file);
    verify_contract(retf == db->size,
        "Unable to read data from file \'%s\'\n", filename);

    // Меняем эндианность ключей.
    for (uint32_t i = 0; i < db->size; ++i)
    {
        db->entries[i].key = my_be32toh(db->entries[i].key);
    }

    // Закрываем файл.
    ret = fclose(file);
    verify_contract(ret != EOF,
        "Detected error on file close operation \'%s\'\n", filename);
}