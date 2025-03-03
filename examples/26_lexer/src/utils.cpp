// Copyright 2025 Vladislav Aleinik
#include <utils.hpp>

#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <cstdarg>
#include <memory>

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
// - В случае невыполнения условия функция бросает исключение, содержащее сообщение об ошибке.
//==================================================================================================
void verify_contract(bool condition, const char* format, ...)
{
    if (!condition) {
        // Инициализируем список аргументов функции.
        std::va_list args1;
        va_start(args1, format);

        // Создаём копию списка аргументов.
        std::va_list args2;
        va_copy(args2, args1);

        // Вычисляем размер строки вывода (+1 для '\0').
        int size_int = std::vsnprintf(nullptr, 0, format, args1) + 1;
        va_end(args1);

        // Проверяем размер форматтированного вывода.
        if (size_int <= 0)
        {
            throw std::runtime_error("verify_contract(): unable to perform formatted output");
        }

        // Выделяем массив необходимого размера.
        auto size = static_cast<size_t>(size_int);
        std::unique_ptr<char[]> buf(new char[size]);

        // Производим запись данных в выделенный буфер.
        std::vsnprintf(buf.get(), size, format, args2);
        va_end(args2);

        // Создаём строку на основе буфера
        auto str = std::string(buf.get(), buf.get() + size - 1);

        // Бросаем исключение на основе созданной строки.
        throw std::runtime_error(str);
    }
}
