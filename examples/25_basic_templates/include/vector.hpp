// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_VECTOR_HPP_INCLUDED
#define HEADER_GUARD_VECTOR_HPP_INCLUDED

#include <cstdlib>

// Производим предварительную декларацию класса Matrix.
namespace MatrixArithmetics
{
    template <class Value_t>
    class Matrix;
}; // namespace MatrixArithmetics

namespace VectorArithmetics
{
    // Класс для представления вектора в N-мерном пространстве.
    template <class Value_t>
    class Vector
    {
    public:
        // Конструкторы и деструктор.
        Vector(size_t size);
        Vector(const Vector<Value_t>& that);
        Vector(Vector<Value_t>&& that);
        ~Vector();

        // Копирование объектов.
        Vector& operator=(const Vector<Value_t>& that);
        Vector& operator=(Vector<Value_t>&& that);

        // Доступ к элементам вектора.
        Value_t& operator[](size_t index);
        size_t size() const;

        // Арифметические операции.
        Vector& operator+=(const Vector<Value_t>& that);

        // Операции сравнения.
        bool operator==(const Vector<Value_t>& that) const;
        bool operator!=(const Vector<Value_t>& that) const;

    private:
        // Обозначаем конструктор без аргмуентов как приватный.
        Vector();
        // Для доступа к нему деларируем friend-класс.
        friend class MatrixArithmetics::Matrix<Value_t>;

        // Поля класса.
        // Инвариант структуры данных:
        // - Указатель data_ указывает на выделенную область динамической память размера size_ * sizeof(Value_t).
        Value_t* data_;
        size_t size_;
    };

    template <class Value_t>
    Vector<Value_t> operator+(const Vector<Value_t>& first, const Vector<Value_t>& second);

}; // namespace VectorArithmetic

// Подключаем заголовочный файл с реализацией шаблонного класса.
#include <vector_impl.hpp>

#endif // HEADER_GUARD_VECTOR_HPP_INCLUDED
