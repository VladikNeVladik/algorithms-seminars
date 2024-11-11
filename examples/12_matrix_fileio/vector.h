// Copyright 2024 Vladislav Aleinik
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "utils.h"

//==================//
// Структуры данных //
//==================//

// Представление вектора в N-мерном пространстве
struct Vector
{
    // Элементы вектора
    double* data;
    // Размерность пространства (кол-во элементов)
    size_t size;
};

//======================//
// Управление ресурсами //
//======================//

//==================================================================================================
// Функция: vector_alloc
// Назначение: Аллоцирует вектор
//--------------------------------------------------------------------------------------------------
// Параметры:
// vec (in/out) - вектор, который требуется инициализировать.
// size (in)    - размерность пространства.
//
// Возвращаемое значение:
// Код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Для каждого вектора, инициализируемого с помощью vector_alloc,
//   должна быть вызвана функция vector_free.
// - Элементы вектора не инициаллизированы
//==================================================================================================
RetCode vector_alloc(struct Vector* vec, size_t size)
{
    if (vec == NULL)
    {
        return RET_INVAL;
    }

    double* data = (double*) calloc(size, sizeof(double));
    if (data == NULL)
    {
        return RET_NOMEM;
    }

    vec->data = data;
    vec->size = size;

    return RET_OK;
}

//==================================================================================================
// Функция: vector_free
// Назначение: Освобождает ресурсы, выделенные под вектор
//--------------------------------------------------------------------------------------------------
// Параметры:
// vec (in/out) - вектор, ресурсы которой требуется освободить.
//
// Возвращаемое значение:
// Код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Для каждого вектора, освобождамого с помощью vector_free,
//   ранее должна быть вызвана функция vector_alloc.
//==================================================================================================
RetCode vector_free(struct Vector* vec)
{
    if (vec == NULL || vec->data == NULL)
    {
        return RET_INVAL;
    }

    free(vec->data);

    // Зануляем указатель, чтобы защититься от повторного вызова vector_free.
    vec->data = NULL;

    return RET_OK;
}

//============================//
// Доступ к элементам вектора //
//============================//

//==================================================================================================
// Функция: vector_get
// Назначение: Возвращает элемент вектора по индексу.
//--------------------------------------------------------------------------------------------------
// Параметры:
// vec (in/out)  - вектор, элемент которого будет извлечён.
// index (in)    - индекс элемента.
// element (out) - указатель, по которому будет записан извлечённый элемент.
//
// Возвращаемое значение:
// Код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Доступ по индексам, не меньшим размера вектора, запрещён.
//==================================================================================================
RetCode vector_get(const struct Vector* vec, size_t index, double* element)
{
    if (vec == NULL || vec->data == NULL || index >= vec->size)
    {
        return RET_INVAL;
    }

    *element = vec->data[index];

    return RET_OK;
}

//==================================================================================================
// Функция: vector_set
// Назначение: Перезаписывает элемент вектора по индексу.
//--------------------------------------------------------------------------------------------------
// Параметры:
// vec (in/out) - вектор, элемент которого будет перезаписан.
// index (in)   - индекс элемента.
// element (in) - элемент, который будет записан в вектор.
//
// Возвращаемое значение:
// Код возврата.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Доступ по индексам, не меньшим размера вектора, запрещён.
//==================================================================================================
RetCode vector_set(struct Vector* vec, size_t index, double element)
{
    if (vec == NULL || vec->data == NULL || index >= vec->size)
    {
        return RET_INVAL;
    }

    vec->data[index] = element;

    return RET_OK;
}
