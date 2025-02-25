#include <builder.hpp>
#include <format>

int main()
{
    Person p = Person::create()
       .works()
       .at("HSE")
       .as("programmer")
       .lives()
       .at("Pokrovskij blvd.");

    std::cout << p << std::endl;

    return 0;
}
