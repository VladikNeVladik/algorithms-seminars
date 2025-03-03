// Copyright 2025 Vladislav Aleinik
#include <expression.hpp>

#include <string>
#include <cstring>

#include <lexer.hpp>

int main(int argc, char* argv[])
{
    if (argc != 3 || std::strcmp(argv[1], "--eval") != 0)
    {
        printf("Usage: eval --eval <expression>\n");
        return EXIT_FAILURE;
    }

    // Создаём лексический анализатор для разбора выражения.
    Lexer lexer{std::string(argv[2])};

    // Обрабатываем лексемы по одной.
    Token token = lexer.getNextToken();
    while (token.type != TOK_EOF)
    {
        printf("Token \'%s\'\n", token.lexeme.c_str());

        token = lexer.getNextToken();
    }

    return EXIT_SUCCESS;
}
