// Copyright 2025 Georgiy Ishchenko & Vladislav Aleinik
#include <parser.hpp>

#include <stdexcept>

Parser::Parser(Lexer& lexer) :
    lexer_ (lexer),
    currentToken_ (),
    previousToken_ ()
{
    // Производим считывание первой лексемы.
    advance();
}

void Parser::advance()
{
    // Считываем следующую лексему.
    previousToken_ = currentToken_;
    currentToken_ = lexer_.getNextToken();
#ifndef NDEBUG
    std::cout << currentToken_.lexeme << std::endl;
#endif
}

void Parser::expect(std::set<TokenType> types)
{
    if (!types.contains(currentToken_.type))
    {
        // Выбрасываем сообщение об ошибке.
        throw std::runtime_error(
            "Got unexpected token \"" + currentToken_.lexeme +
            "\" of type " + std::to_string(currentToken_.type));
    }

    // В случае успеха переходим к следующей лексеме.
    advance();
}

bool Parser::match(TokenType type)
{
    if (currentToken_.type == type)
    {
        advance();
        return true;
    }

    return false;
}

Expression Parser::parseExpression()
{
    Expression expr = parseExpr();

    expect({TOK_EOF});

    return expr;
}

Expression Parser::parseExpr()
{
#ifndef NDEBUG
    std::cout << "Expr" << std::endl;
#endif
    Expression expr = parseTerm();

    while (currentToken_.type == TOK_PLUS)
    {
        // Считываем символ '+' в обязательном порядке.
        advance();

        // Считываем следующее слагаемое в выражении.
        Expression term = parseTerm();

        // Обновляем выражение для суммы.
        expr += term;
    }

    return expr;
}

Expression Parser::parseTerm()
{
#ifndef NDEBUG
    std::cout << "Term" << std::endl;
#endif
    Expression term = parseFactor();

    while (currentToken_.type == TOK_MULTIPLY)
    {
        // Считываем символ '*' в обязательном порядке.
        advance();

        // Считываем следующий множитель в выражении.
        Expression factor = parseFactor();

        // Обновляем выражение для произведения.
        term *= factor;
    }

    return term;
}

Expression Parser::parseFactor()
{
#ifndef NDEBUG
    std::cout << "Factor" << std::endl;
#endif
    if (match(TOK_BRACKET_LEFT))
    {
        Expression expr = parseExpr();

        expect({TOK_BRACKET_RIGHT});

        return expr;
    }

    if (match(TOK_VALUE))
    {
        return Expression(std::stold(previousToken_.lexeme));
    }

    if (match(TOK_VARIABLE))
    {
        return Expression(previousToken_.lexeme);
    }

    throw std::runtime_error(
        "Got unexpected token \"" + currentToken_.lexeme +
        "\" of type " + std::to_string(currentToken_.type));
}
