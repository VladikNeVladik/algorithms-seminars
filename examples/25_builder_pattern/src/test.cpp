// Copyright 2025 Sergei Afonin & Vladislav Aleinik
#include <builder.hpp>
#include <format>

int main()
{
    PersonBuilder pb = Person::create();

    pb.works().at("HSE").as("programmer");

    pb.lives().at("Pokrovskij blvd.");

    std::cout << (Person) pb << std::endl;

    return 0;
}
