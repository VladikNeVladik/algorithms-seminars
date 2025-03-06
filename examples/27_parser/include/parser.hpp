// Copyright 2025 Georgiy Ishchenko & Vladislav Aleinik
#ifndef HEADER_GUARD_PARSER_HPP_INCLUDED
#define HEADER_GUARD_PARSER_HPP_INCLUDED

#include <set>

#include <expression.hpp>
#include <lexer.hpp>

// Задание формальной грамматики языка выражений.
// Терминалы - лексемы языка выражений: Value, Variable, +, *, (, )
//
// Правила продукций в форме Бэкуса-Наура.
// Expr   ::= Term + Expr
//          | Term
//
// Term   ::= Factor * Term
//          | Factor
//
// Factor ::= ( Expr )
//          | Value
//          | Variable

// Синтаксический анализатор для синтаксического разбора языка выражений.
class Parser
{
public:
    // Создание парсера на основе лексера.
    Parser(Lexer& lexer);

    // Синтаксический разбор выражения.
    Expression parseExpression();

private:
    // Ссылка на лексический анализатор.
    Lexer& lexer_;
    // Текущая лексема.
    Token currentToken_;
    // Предыдущая считанная лексема.
    Token previousToken_;

    // Безусловный cдвиг "каретки" синтаксического анализатора.
    void advance();
    // Сдвиг "каретки" синтаксического анализатора с выдачей исключения при несовпадении типа лексемы.
    void expect(std::set<TokenType> types);
    // Сдвиг "каретки" синтаксического анализатора при совпадении типа лексемы.
    bool match(TokenType type);

    // Методы для синтаксического разбора согласно формальной грамматике.
    Expression parseExpr();
    Expression parseTerm();
    Expression parseFactor();
};

#endif // HEADER_GUARD_PARSER_HPP_INCLUDED
