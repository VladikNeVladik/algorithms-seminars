// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_VECTOR_IMPL_HPP_INCLUDED
#define HEADER_GUARD_VECTOR_IMPL_HPP_INCLUDED

#include <metaprogramming.hpp>

#include <utils.hpp>

#include <cstdlib>
#include <algorithm>
#include <cmath>

namespace VectorArithmetics
{
//---------------------------
// Конструкторы и деструктор
//---------------------------

template <class Value_t>
Vector<Value_t>::Vector() :
    data_ (nullptr),
    size_ (0)
{}

template <class Value_t>
Vector<Value_t>::Vector(size_t size) :
    data_ (nullptr),
    size_ (size)
{
    static_assert(std::is_default_constructible<Value_t>::value,
        "Vector constructor requires a default constructible type!");

    // Оператор new бросает исключение std::bad_alloc при возникновении ошибки аллокации.
    data_ = new Value_t[size];
}

template <class Value_t>
Vector<Value_t>::Vector(const Vector<Value_t>& that) :
    data_ (nullptr),
    size_ (that.size_)
{
    static_assert(std::is_default_constructible<Value_t>::value,
        "Vector copy constructor requires a default constructible type!");

    // Аллоцируем новый массив нужного размера.
    data_ = new Value_t[size_];

    // Производим глубокое копирование элементов.
    std::copy_n(that.data_, size_, data_);
}

template <class Value_t>
Vector<Value_t>::Vector(Vector<Value_t>&& that)
{
    std::swap(data_, that.data_);
    std::swap(size_, that.size_);
}

template <class Value_t>
Vector<Value_t>::~Vector()
{
    static_assert(std::is_destructible<Value_t>::value,
        "Vector destructor requires a destructible type!");

    if (data_ != nullptr)
    {
        delete[] data_;
    }

    // Выставляем значение указателя в NULL для детектирования повторного вызова деструктора.
    data_ = nullptr;
}

//-----------------------
// Оператор присваивания
//-----------------------

template <class Value_t>
Vector<Value_t>& Vector<Value_t>::operator=(const Vector<Value_t>& that)
{
    static_assert(std::is_default_constructible<Value_t>::value,
        "Vector copy assignment requires a default constructible type!");

    if (this == &that)
    {
        // В случае присваивания самому себе никакие действия не требуются.
        return *this;
    }

    // Производим реаллокацию имеющегося диапозона памяти.
    if (data_ != nullptr)
    {
        delete[] data_;
    }

    data_ = new Value_t[that.size_];

    // Копируем размер.
    size_ = that.size_;

    // Производим глубокое копирование элементов.
    std::copy_n(that.data_, size_, data_);

    return *this;
}

template <class Value_t>
Vector<Value_t>& Vector<Value_t>::operator=(Vector<Value_t>&& that)
{
    static_assert(std::is_destructible<Value_t>::value,
        "Vector move assignment requires a destructible type!");

    std::swap(data_, that.data_);
    std::swap(size_, that.size_);

    return *this;
}

//----------------------------
// Доступ к элементам вектора
//----------------------------

template <class Value_t>
Value_t& Vector<Value_t>::operator[](size_t index)
{
    verify_contract(data_ != nullptr,
        "Invalid vector data (use-after-free)\n");
    verify_contract(index < size_,
        "Index %zu out of bounds (size is %zu)\n",
        index, size_);

    return data_[index];
}

template <class Value_t>
size_t Vector<Value_t>::size() const
{
    return size_;
}

//-------------------------
// Арифметические операции
//-------------------------

template <class Value_t>
Vector<Value_t>& Vector<Value_t>::operator+=(const Vector<Value_t>& that)
{
    static_assert(Meta::is_addition_assignable<Value_t>::value,
        "Vector::operator+= requires an addition-assignable parameter type!");

    verify_contract(data_ != nullptr,
        "Invalid vector data (possible double free for left argument)\n");
    verify_contract(that.data_ != nullptr,
        "Invalid vector data (possible double free for right argument)\n");
    verify_contract(size_ == that.size_,
        "Unmatched operand sizes (%zu and %zu)\n",
        size_, that.size_);

    for (size_t i = 0U; i < size_; ++i)
    {
        data_[i] += that.data_[i];
    }

    return *this;
}

template <class Value_t>
Vector<Value_t> VectorArithmetics::operator+(const Vector<Value_t>& first, const Vector<Value_t>& second)
{
    Vector<Value_t> copy{first};

    copy += second;

    return copy;
}

//--------------------
// Операции сравнения
//--------------------

template <class Value_t>
bool Vector<Value_t>::operator==(const Vector<Value_t>& that) const
{
    static_assert(Meta::is_comparable<Value_t>::value,
        "Vector::operator== requires a comparable parameter type!");

    verify_contract(data_ != nullptr,
        "Invalid vector data (possible double free for left argument)\n");
    verify_contract(that.data_ != nullptr,
        "Invalid vector data (possible double free for right argument)\n");

    // В случае совпадающих объектов проводить полное сравнение не имеет смысла.
    if (this == &that)
    {
        return true;
    }

    // Равенство определено для векторов из одного векторного пространства.
    if (size_ != that.size_)
    {
        return false;
    }

    for (size_t i = 0U; i < size_; ++i)
    {
        if (data_[i] != that.data_[i])
        {
            return false;
        }
    }

    return true;
}

template <class Value_t>
bool Vector<Value_t>::operator!=(const Vector<Value_t>& that) const
{
    return !(*this == that);
}

}; // namespace VectorArithmetic

#endif // HEADER_GUARD_VECTOR_IMPL_HPP_INCLUDED
