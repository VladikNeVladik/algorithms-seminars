// Copyright 2025 Vladislav Aleinik
#include <utils.hpp>

//==================================================================================================
// Функция: verify_contract
// Назначение:
// Производит проверку входных данных. В случае ошибки выводит сообщение об ошибке.
//--------------------------------------------------------------------------------------------------
// Параметры:
// condition (in) - условие, которое требуется проверять.
// format    (in) - формат сообщения об ошибке (см. printf).
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
//==================================================================================================
void verify_contract(bool condition, const char* format, ...)
{
    if (!condition) {
        va_list vargs;
        va_start(vargs, format);
        vprintf(format, vargs);
        va_end(vargs);
        exit(EXIT_FAILURE);
    }
}

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
        case RetCode::RET_OK:
        {
            return "everything is ok";
        }
        case RetCode::RET_NOMEM:
        {
            return "not enough memory";
        }
        case RetCode::RET_INVAL:
        {
            return "invalid argument";
        }
        case RetCode::RET_FILEIO:
        {
            return "failed file operation";
        }
        default:
        {
            return "invalid return code";
        }
    }
}
