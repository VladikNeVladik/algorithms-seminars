// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_MATRIX_HPP_INCLUDED
#define HEADER_GUARD_MATRIX_HPP_INCLUDED

#include <string>
#include <vector.hpp>

namespace MatrixArithmetics
{
    using namespace VectorArithmetics;

    // Класс для представления двумерной матрицы NxM.
    template <class Value_t>
    class Matrix
    {
    public:
        // Конструкторы и деструктор.
        Matrix() = delete;
        Matrix(size_t size_x, size_t size_y);
        Matrix(const std::string& filename);

        Matrix(const Matrix<Value_t>& that);
        Matrix(Matrix<Value_t>&& that);

        ~Matrix();

        // Копирование объектов.
        Matrix<Value_t>& operator=(const Matrix<Value_t>& that);
        Matrix<Value_t>& operator=(Matrix<Value_t>&& that);

        // Доступ к элементам матрицы.
        Vector<Value_t>& operator[](size_t index);

        // Арифметические операции.
        Matrix<Value_t>& operator+=(const Matrix<Value_t>& that);

        // Операции сравнения.
        bool operator==(const Matrix<Value_t>& that) const;
        bool operator!=(const Matrix<Value_t>& that) const;

        // Запись матрицы в файл.
        Matrix<Value_t>& dump_to_file(const std::string& filename);

    private:
        Vector<Value_t>* data_;
        size_t size_x_;
        size_t size_y_;
    };

    template <class Value_t>
    Matrix<Value_t> operator+(const Matrix<Value_t>& first, const Matrix<Value_t>& second);

}; // namespace MatrixArithmetics

// Подключаем заголовочный файл с реализацией шаблонного класса.
#include <matrix_impl.hpp>

#endif // HEADER_GUARD_MATRIX_HPP_INCLUDED
