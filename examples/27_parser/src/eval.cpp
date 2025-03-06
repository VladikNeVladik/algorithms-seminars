// Copyright 2025 Vladislav Aleinik
#include <expression.hpp>

#include <string>
#include <cstring>

#include <lexer.hpp>
#include <parser.hpp>

int main(int argc, char* argv[])
{
    if (argc != 3 || std::strcmp(argv[1], "--eval") != 0)
    {
        printf("Usage: eval --eval <expression>\n");
        return EXIT_FAILURE;
    }

    // Создаём лексический анализатор для разбора выражения.
    Lexer lexer{std::string(argv[2])};

    // Создаём парсер для выражения.
    Parser parser{lexer};

    // Создаём выражение на основе строки.
    Expression expr = parser.parseExpression();

    // Задаём контексты для вычисления выражений.
    std::map<std::string, Value_t> context1 =
    {
        {"x", 1.0}, {"y", 2.0}
    };

    std::map<std::string, Value_t> context2 =
    {
        {"x", 2.0}, {"y", 3.0}
    };

    // Вычисляем выражение, созданное на основе строки, в разных контекстах.
    printf("EVAL[%s]{x = 1.0, y = 2.0} = %Lf\n", expr.to_string().c_str(), expr.eval(context1));
    printf("EVAL[%s]{x = 2.0, y = 3.0} = %Lf\n", expr.to_string().c_str(), expr.eval(context2));

    return EXIT_SUCCESS;
}
