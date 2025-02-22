// Copyright 2024 Vladislav Aleinik
#include <vector.hpp>
#include <matrix.hpp>

#include <test_system.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>

//-------------------------
// Вспомогательная функция
//-------------------------

template <class Value_t>
void fill_vector(VectorArithmetics::Vector<Value_t>& vector)
{
    throw std::runtime_error("Not implemented!");
}

template <>
void fill_vector<double>(VectorArithmetics::Vector<double>& vector)
{
    for (size_t i = 0U; i < vector.size(); ++i)
    {
        vector[i] = 1.0 / i;
    }
}

template <>
void fill_vector<int>(VectorArithmetics::Vector<int>& vector)
{
    for (size_t i = 0U; i < vector.size(); ++i)
    {
        vector[i] = i;
    }
}

//----------------------------
// Тестовый набор для вектора
//----------------------------

constexpr size_t SIZE = 100U;

using namespace TestSystem;

template <class Value_t>
bool test_constructor()
{
    using namespace VectorArithmetics;

    try
    {
        Vector<Value_t> vec1{0};
        Vector<Value_t> vec2{SIZE};
    }
    catch (const std::exception& exc)
    {
        return FAIL;
    }

    return OK;
}

template <class Value_t>
bool test_copy_constructor()
{
    using namespace VectorArithmetics;

    Vector<Value_t> vec1{SIZE};
    fill_vector(vec1);

    Vector<Value_t> vec2{vec1};

    return (vec1 == vec2)? OK : FAIL;
}

template <class Value_t>
bool test_move_constructor()
{
    using namespace VectorArithmetics;

    Vector<Value_t> vec1{SIZE};
    Vector<Value_t> vec2{SIZE};

    fill_vector(vec1);
    fill_vector(vec2);

    Vector<Value_t> vec3{std::move(vec1)};

    return (vec2 == vec3)? OK : FAIL;
}

template <class Value_t>
bool test_copy_assignment()
{
    using namespace VectorArithmetics;

    Vector<Value_t> vec1{SIZE};
    fill_vector(vec1);

    Vector<Value_t> vec2{2*SIZE};

    vec2 = vec1;

    return (vec1 == vec2)? OK : FAIL;
}

template <class Value_t>
bool test_move_assignment()
{
    using namespace VectorArithmetics;

    Vector<Value_t> vec1{SIZE};
    Vector<Value_t> vec2{SIZE};

    fill_vector(vec1);
    fill_vector(vec2);

    Vector<Value_t> vec3{2*SIZE};

    vec3 = std::move(vec1);

    return (vec2 == vec3)? OK : FAIL;
}

template <class Value_t>
bool test_addition()
{
    using namespace VectorArithmetics;

    // Создаём векторы и заполняем их элементами.
    Vector<Value_t> vec2{SIZE};
    Vector<Value_t> vec1{SIZE};

    for (size_t i = 0U; i < SIZE; ++i)
    {
        vec1[i] = (Value_t) i;
        vec2[i] = (Value_t) 2*i;
    }

    // Складываем два векора вместе.
    vec1 = vec1 + vec1;

    return (vec1 != vec2)? FAIL : OK;
}

//-----------------
// Тестовые наборы
//-----------------

int main(void)
{
    printf("Test Vector<double> operations\n");
    run_test("constructor",      test_constructor<double>);
    run_test("copy-constructor", test_copy_constructor<double>);
    run_test("move-constructor", test_move_constructor<double>);
    run_test("copy-assignment",  test_copy_assignment<double>);
    run_test("move-assignment",  test_move_assignment<double>);
    run_test("addition",         test_addition<double>);

    printf("Test Vector<int> operations\n");
    run_test("constructor",      test_constructor<int>);
    run_test("copy-constructor", test_copy_constructor<int>);
    run_test("move-constructor", test_move_constructor<int>);
    run_test("copy-assignment",  test_copy_assignment<int>);
    run_test("move-assignment",  test_move_assignment<int>);
    run_test("addition",         test_addition<int>);

    return EXIT_SUCCESS;
}
