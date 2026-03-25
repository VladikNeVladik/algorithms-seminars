#include <stdint.h>

struct obj;

// Таблица виртуальных функций.
struct vtable
{
    void* (*foo)(struct obj*, __float128 arg);
};

// Бинарное предстваление экземпляр класса без полей, но с виртуальными функциями.
struct obj
{
    // Указатель на таблицу виртуальных функций
    // лежит первым аргументом структуры.
    struct vtable* vtable;
};

// Реализация вызова виртуальной функции.
static void* foo(struct obj* self, __float128 arg)
{
    // Сложный вызов по указателю нужен, чтобы запретить автоматичекую подстановку функций
    // (inline-инг) даже при высоких уровнях оптимизации.
    return self->vtable->foo(self, arg);
}

// Конкретноая реализация виртуальной функции.
static void* real_foo(struct obj* self, __float128 arg)
{
    return 0;
}

// Таблица виртуальных функций для класса obj.
static struct vtable vtable =
{
    .foo = real_foo,
};

// Экземпляр класса obj.
static struct obj obj =
{
    .vtable = &vtable,
};

int segfault(void)
{
    // Создаём переменную, к которой компилятор захочет обращаться
    // через SSE2 инструкции, требующие выравнивания.
    __float128 arg;

    // Вызов функции foo будет размещать аргументы
    // на стеке при помощи инструкции MOVAPS.
    foo(&obj, arg);

    return 0;
}
