// Copyright 2025 Georgiy Ishchenko & Vladislav Aleinik
#ifndef HEADER_GUARD_LEXER_HPP_INCLUDED
#define HEADER_GUARD_LEXER_HPP_INCLUDED

#include <iostream>
#include <string>
#include <regex>

// Тип лексемы языка выражений.
enum TokenType
{
    // Ошибка в разборе строки.
    TOK_ERROR = 0,
    // Числовое значение.
    TOK_VALUE = 1,
    // Символьная переменная.
    TOK_VARIABLE = 2,
    // Операция '+'.
    TOK_PLUS = 3,
    // Операция '*'.
    TOK_MULTIPLY = 4,
    // Левая и правая круглые скобки.
    TOK_BRACKET_LEFT  = 5,
    TOK_BRACKET_RIGHT = 6,
    // Конец файла.
    TOK_EOF = 7
};

// Таблица регулярных выражений для типов лексем.
constexpr const char* REGEXP_SPACE_SEQUENCE = "[ \t]+";
constexpr const char* REGEXP_VARIABLE       = "[a-zA-Z_]+";
constexpr const char* REGEXP_VALUE          = "(\\+|-)?(0|[1-9][0-9]*)(\\.[0-9]+)?";

// Лексема языка выражений, а также её метаинформация.
struct Token
{
    TokenType type;
    std::string lexeme;
    size_t column;
};

// Лексический анализатор для лексического разбора языка выражений.
class Lexer
{
public:
    // Создание лексера для разбора строки.
    Lexer(const std::string& input);

    // Создание, удаление, копирование и перемещение лексического анализатора.
    Lexer()  = delete;
    ~Lexer() = default;

    Lexer(const Lexer&) = delete;
    Lexer(Lexer&&) = default;

    Lexer& operator=(const Lexer&) = delete;
    Lexer& operator=(Lexer&&) = default;

    // Извлечение следующей лексемы строки.
    Token getNextToken();

private:
    // Текст для лексического разбора.
    std::string input_;
    // Текущая позиция в разбираемом тексте.
    const char* pos_;
    // Финальная позиция в разбираемом тексте.
    const char* end_;
    // Номер символа в строке разбираемого текста.
    size_t column_;

    // Просмотр следующего символа текста без извлечения.
    char peek() const;

    // Извлечение следующего символа текста.
    char get();

    // Считывание лексемы, заданной регулярным выражением.
    Token getTokenByPattern(const std::regex& regexp, TokenType expectedTokenType);
    // Считывание лексемы, заданной регулярным выражением, без сохранения.
    void skipTokenByPattern(const std::regex& regexp);

    // Пропуск последовательности пробельных символов.
    void skipSpaceSequence();
    // Считывание переменной.
    Token getVariable();
    // Считывание числового значения.
    Token getValue();
};

#endif // HEADER_GUARD_LEXER_HPP_INCLUDED
