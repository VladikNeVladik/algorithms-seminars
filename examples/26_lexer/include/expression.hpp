// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_EXPRESSION_HPP_INCLUDED
#define HEADER_GUARD_EXPRESSION_HPP_INCLUDED

#include <string>
#include <map>
#include <memory>

// Тип, параметризующий класс выражение.
typedef long double Value_t;

// Абстрактный класс, задающий интерфейс между выражением и его реализацией.
class ExpressionImpl
{
public:
    // Запрет на создание экземпляров класса ExpressionImpl.
    ExpressionImpl() = default;
    virtual ~ExpressionImpl() = default;

    // Функция вычисления результата выражения.
    virtual Value_t eval(std::map<std::string, Value_t> context) const = 0;

    // Функция преобразования выражения в строку.
    virtual std::string to_string() const = 0;
};

// Класс, задающий выражение и методы работы с ним.
class Expression
{
public:
    // Создание выражений.
    Expression(std::string variable);

    // Конструирование выражений на основе базовых типов C++.
    friend Expression operator""_val(Value_t val);
    friend Expression operator""_var(const char* variable);
    friend Expression operator""_var(const char* variable, size_t size);

    // Конструирование выражений на основе других выражений.
    Expression  operator+ (const Expression& that);
    Expression& operator+=(const Expression& that);
    Expression  operator* (const Expression& that);
    Expression& operator*=(const Expression& that);

    // Операции с выражениями.
    Value_t eval(std::map<std::string, Value_t> context) const;
    std::string to_string() const;

private:
    Expression(std::shared_ptr<ExpressionImpl> impl);

    std::shared_ptr<ExpressionImpl> impl_;
};

Expression operator""_val(Value_t val);
Expression operator""_var(const char* variable);
Expression operator""_var(const char* variable, size_t size);

// Класс, представляющий число в рамках выражения.
class Value : public ExpressionImpl
{
public:
    // Создание числа на основе ... числа.
    Value(Value_t value);

    virtual ~Value() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> context) const override;
    virtual std::string to_string() const override;

private:
    Value_t value_;
};

// Класс, представляющий переменную в рамках выражения.
class Variable : public ExpressionImpl
{
public:
    // Создание переменной на основе её имени.
    Variable(std::string name);

    virtual ~Variable() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> context) const override;
    virtual std::string to_string() const override;

private:
    std::string name_;
};

// Класс, представляющий выражение сложения двух выражений.
class OperationAdd : public ExpressionImpl
{
public:
    // Создание выражения для суммы на основе подвыражений.
    OperationAdd(Expression left, Expression right);

    virtual ~OperationAdd() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> context) const override;
    virtual std::string to_string() const override;

private:
    Expression left_;
    Expression right_;
};

// Класс, представляющий выражение умножения двух выражений.
class OperationMul : public ExpressionImpl
{
public:
    // Создание выражения для суммы на основе подвыражений.
    OperationMul(Expression left, Expression right);

    virtual ~OperationMul() override = default;

    // Реализация интерфейса ExpressionImpl.
    virtual Value_t eval(std::map<std::string, Value_t> context) const override;
    virtual std::string to_string() const override;

private:
    Expression left_;
    Expression right_;
};

#endif // HEADER_GUARD_EXPRESSION_HPP_INCLUDED
