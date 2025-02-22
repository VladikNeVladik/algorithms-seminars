// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_MATRIX_IMPL_HPP_INCLUDED
#define HEADER_GUARD_MATRIX_IMPL_HPP_INCLUDED

#include <utils.hpp>

#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

namespace MatrixArithmetics
{

//---------------------------
// Конструкторы и деструктор
//---------------------------

template <class Value_t>
Matrix<Value_t>::Matrix(size_t size_x, size_t size_y) :
    data_ (nullptr),
    size_x_ (size_x),
    size_y_ (size_y)
{
    // Создаём массив элементов класса Vector, вызывая конструктор по умолчанию.
    data_ = new Vector[size_y];

    for (size_t i = 0U; i < size_y_; ++i)
    {
        // Вызываем оператор присваивания.
        data_[i] = Vector<Value_t>(size_x_);
    }
}

template <class Value_t>
Matrix<Value_t>::Matrix(const std::string& filename)
{
    // Открываем файл, сожержащий текстовое представление матрицы.
    std::ifstream file = std::ifstream(filename, std::ios::in);

    verify_contract(file.good(),
        "Unable to open file \'%s\'\n",
        filename.c_str());

    // Считываем размеры матрицы из файла.
    file >> size_y_;
    file >> size_x_;

    verify_contract(file.good(),
        "Unable to parse matrix sizes \'%s\'\n",
        filename.c_str());

    // Выделяем место под матрицу.
    data_ = new Vector[size_y_];

    for (size_t i = 0U; i < size_y_; ++i)
    {
        // Вызываем оператор присваивания.
        data_[i] = Vector(size_x_);
    }

    // Считываем элементы матрицы из файла.
    for (size_t y = 0U; y < size_y_; ++y)
    {
        for (size_t x = 0U; x < size_x_; ++x)
        {
            double element;
            file >> element;

            verify_contract(file.good(),
                "Unable to parse matrix elements\n");

            data_[y][x] = element;
        }
    }

    // Файл будет автоматически закрыт при вызове деструктора объекта file.
}

template <class Value_t>
Matrix<Value_t>::Matrix(const Matrix& that) :
    data_ (nullptr),
    size_x_ (that.size_x_),
    size_y_ (that.size_y_)
{
    // Выделяем место под матрицу.
    data_ = new Vector[size_y_];

    for (size_t i = 0U; i < size_y_; ++i)
    {
        // Вызываем оператор присваивания.
        data_[i] = Vector(size_x_);
    }

    // Копируем элементы массива.
    std::copy_n(that.data_, size_y_, data_);
}

template <class Value_t>
Matrix<Value_t>::~Matrix()
{
    verify_contract(data_ != nullptr,
        "Invalid vector data (possible double free)\n");

    delete[] data_;

    // Выставляем значение указателя в NULL для детектирования повторного вызова деструктора.
    data_ = nullptr;
}

//-----------------------
// Оператор присваивания
//-----------------------

template <class Value_t>
Matrix<Value_t>& Matrix<Value_t>::operator=(const Matrix<Value_t>& that)
{
    if (this == &that)
    {
        // В случае присваивания самому себе никакие действия не требуются.
        return *this;
    }

    if (that.size_y_ != size_y_)
    {
        // Производим реаллокацию имеющегося диапозона памяти.
        Vector<Value_t>* new_data = new Vector[that.size_y_];

        delete[] data_;

        data_ = new_data;
    }

    // Копируем размер и содержимое массива.
    size_y_ = that.size_y_;
    size_x_ = that.size_x_;

    // Производим глубокое копирование элементов.
    std::copy_n(that.data_, size_y_, data_);

    return *this;
}

//----------------------------
// Доступ к элементам матрицы
//----------------------------

template <class Value_t>
Vector<Value_t>& Matrix<Value_t>::operator[](size_t index)
{
    verify_contract(data_ != nullptr,
        "Invalid matrix data (use-after-free)\n");
    verify_contract(index < size_y_,
        "Index %zu out of bounds (size is %zu)\n",
        index, size_y_);

    return data_[index];
}

//-------------------------
// Арифметические операции
//-------------------------

template <class Value_t>
Matrix<Value_t>& Matrix<Value_t>::operator+=(const Matrix<Value_t>& that)
{
    verify_contract(data_ != nullptr,
        "Invalid matrix data (possible double free for left argument)\n");
    verify_contract(that.data_ != nullptr,
        "Invalid matrix data (possible double free for right argument)\n");
    verify_contract(size_y_ == that.size_y_,
        "Unmatched operand sizes (%zu and %zu)\n",
        size_y_, that.size_y_);

    for (size_t i = 0U; i < size_y_; ++i)
    {
        data_[i] += that.data_[i];
    }

    return *this;
}

template <class Value_t>
Matrix<Value_t> operator+(const Matrix<Value_t>& first, const Matrix<Value_t>& second)
{
    Matrix<Value_t> copy{first};

    copy += second;

    return copy;
}

//--------------------
// Операции сравнения
//--------------------

template <class Value_t>
bool Matrix<Value_t>::operator==(const Matrix<Value_t>& that) const
{
    verify_contract(data_ != nullptr,
        "Invalid matrix data (possible double free for left argument)\n");
    verify_contract(that.data_ != nullptr,
        "Invalid matrix data (possible double free for right argument)\n");

    // В случае совпадающих объектов проводить полное сравнение не имеет смысла.
    if (this == &that)
    {
        return true;
    }

    // Равенство определено для матриц одного размера.
    if (size_y_ != that.size_y_ || size_x_ != that.size_x_)
    {
        return false;
    }

    for (size_t i = 0U; i < size_y_; ++i)
    {
        if (data_[i] != that.data_[i])
        {
            return false;
        }
    }

    return true;
}

template <class Value_t>
bool Matrix<Value_t>::operator!=(const Matrix<Value_t>& that) const
{
    return !(*this == that);
}

//-----------------------
// Запись матрицы в файл
//-----------------------

template <class Value_t>
Matrix<Value_t>& Matrix<Value_t>::dump_to_file(const std::string& filename)
{
    // Открываем файл для записи матрицы.
    std::ofstream file = std::ofstream(filename, std::ios::out);

    verify_contract(file.good(),
        "Unable to open file \'%s\'\n",
        filename.c_str());

    // Записываем размеры матрицы в файл.
    file << size_y_ << " " << size_x_ << std::endl;

    verify_contract(file.good(),
        "Unable to dump matrix sizes to file \'%s\'\n",
        filename.c_str());

    // Записываем элементы матрицы по строка.
    for (size_t y = 0U; y < size_y_; ++y)
    {
        size_t x = 0U;
        for (; x + 1U < size_x_; ++x)
        {
            // Записываем элемент матрицы, используя пробел в качестве разделителя.
            file << data_[y][x] << " ";

            verify_contract(file.good(),
                "Unable to dump matrix element to file \'%s\'\n",
                filename.c_str());
        }

        // Записываем последний элемент матрицы в файл.
        file << data_[y][x] << std::endl;

        verify_contract(file.good(),
            "Unable to dump matrix element to file \'%s\'\n",
            filename.c_str());
    }

    // Файл будет автоматически закрыт при вызове деструктора объекта file.

    return *this;
}

}; // namespace MatrixArithmetics

#endif // HEADER_GUARD_MATRIX_IMPL_HPP_INCLUDED
