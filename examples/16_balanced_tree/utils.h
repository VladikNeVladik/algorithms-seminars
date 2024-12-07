// Copyright 2024 Vladislav Aleinik
#ifndef HEADER_GUARD_UTILS_H_INCLUDED
#define HEADER_GUARD_UTILS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>

//=================//
// Удобные функции //
//=================//

//==================================================================================================
// Функция: min
// Назначение: вычисляет минимальное из двух чисел
//--------------------------------------------------------------------------------------------------
// Параметры:
// a (in) - первый аргумент.
// b (in) - второй аргумент.
//
// Возвращаемое значение:
// Меньшее из двух чисел.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
inline unsigned min(unsigned a, unsigned b)
{
    return a < b? a : b;
}

//==================================================================================================
// Функция: max
// Назначение: вычисляет макисмальное из двух чисел
//--------------------------------------------------------------------------------------------------
// Параметры:
// a (in) - первый аргумент.
// b (in) - второй аргумент.
//
// Возвращаемое значение:
// Меньшее из двух чисел.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
inline unsigned max(unsigned a, unsigned b)
{
    return a > b? a : b;
}

// Точность сравнения чисел с плавающей точкой двойной точности
#define DOUBLE_PRECISION 10e-9

//==================================================================================================
// Функция: double_equal
// Назначение: производит сравнение чисел с плавающей точкой с заданной абсолютной погрешностью
//--------------------------------------------------------------------------------------------------
// Параметры:
// el1 (in) - первое число.
// el2 (in) - второе число.
//
// Возвращаемое значение:
// Флаг равенства чисел в пределах погрешности.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
bool double_equal(double el1, double el2)
{
    return fabs(el1 - el2) < DOUBLE_PRECISION;
}

//==================//
// Обработка ошибок //
//==================//

//==================================================================================================
// Функция: verify_contract
// Назначение:
// Производит проверку входных данных. В случае ошибки выводит сообщение об ошибке.
//--------------------------------------------------------------------------------------------------
// Параметры:
// condition (in) - условие, которое требуется проверять.
// format (in) - формат сообщения об ошибке (см. printf).
// ... - дополнительные аргументы, соответствующие формату (см. printf).
//
// Возвращаемое значение:
// отсутствует
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - В случае невыполнения условия функция печатает сообщение об ошибке
//   и завершает выполнение программы.
// - Автор реализации - Максим Кузнецов.
//==================================================================================================
void verify_contract(bool condition, const char* restrict format, ...)
{
    if (!condition) {
        va_list vargs;
        va_start(vargs, format);
        vprintf(format, vargs);
        va_end(vargs);
        exit(EXIT_FAILURE);
    }
}

// Тип данных, задающий код возврата
typedef enum
{
    // Операция выполнена без ошибок
    RET_OK   = 0,
    // Операция не выполнена, т.к. для её выполнения не хватает памяти
    RET_NOMEM = 1,
    // Операция не выполнена, т.к. аргументы операции некорректны
    RET_INVAL = 2,
    // Операция не выполнена, т.к. операция с файлом вернула ошибку
    RET_FILEIO = 3
} RetCode;

//==================================================================================================
// Функция: retcode_str
// Назначение: Возвращает строку, соответствующую коду ошибки
//--------------------------------------------------------------------------------------------------
// Параметры:
// code (in) - код возврата, которому требуется сопоставить текстовое сообщение.
//
// Возвращаемое значение:
// Строка, соответствующая коду возврата code.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================
const char* retcode_str(RetCode code)
{
    switch (code)
    {
        case RET_OK:
        {
            return "everything is ok";
        }
        case RET_NOMEM:
        {
            return "not enough memory";
        }
        case RET_INVAL:
        {
            return "invalid argument";
        }
        case RET_FILEIO:
        {
            return "failed file operation";
        }
        default:
        {
            return "invalid return code";
        }
    }
}

//==================================================================================================
// Макрос: STATIC_ASSERT
// Назначение:
// Производит проверку выражения на этапе сборки программы
//--------------------------------------------------------------------------------------------------
// Параметры:
// COND (in) - условие, которое требуется проверять.
// MSG  (in) - идентификатор сообщения об ошибке.
//
// Примечания:
// - В случае невыполнения условия COND будет сгенерирована ошибка сборки.
//==================================================================================================
#define STATIC_ASSERT(COND, MSG) \
    typedef char static_assertion_##MSG[(COND)?1:-1];

#endif // HEADER_GUARD_UTILS_H_INCLUDED
