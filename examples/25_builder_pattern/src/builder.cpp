// Copyright 2025 Sergei Afonin & Vladislav Aleinik
#include <format>

#include <builder.hpp>

//--------------//
// Класс Person //
//--------------//

PersonBuilder Person::create()
{
    return PersonBuilder();
}

std::ostream& operator<<(std::ostream& os, const Person& p)
{
    os << std::format(
            "Home: {}\nWork: {} as {}",
            p.street_address,
            p.company_name,
            p.position);
    return os;
}

//-------------------------//
// Класс PersonBuilderBase //
//-------------------------//

PersonBuilderBase::PersonBuilderBase(Person& person) :
    person (person) {}

PersonBuilderBase::operator Person()
{
    return std::move(person);
}

PersonAddressBuilder PersonBuilderBase::lives() const
{
    return PersonAddressBuilder(person);
}

PersonJobBuilder PersonBuilderBase::works() const
{
    return PersonJobBuilder(person);
}

//---------------------//
// Класс PersonBuilder //
//---------------------//

PersonBuilder::PersonBuilder() :
    PersonBuilderBase(p) {}

//----------------------------//
// Класс PersonAddressBuilder //
//----------------------------//

PersonAddressBuilder::PersonAddressBuilder(Person & person) :
    PersonBuilderBase(person)
{}

PersonAddressBuilder& PersonAddressBuilder::city(std::string city)
{
    person.city = city;
    return *this;
}

PersonAddressBuilder& PersonAddressBuilder::at(std::string street_address)
{
    person.street_address = street_address;
    return *this;
}

//------------------------//
// Класс PersonJobBuilder //
//------------------------//

PersonJobBuilder::PersonJobBuilder(Person& person) :
    PersonBuilderBase(person)
{}

PersonJobBuilder& PersonJobBuilder::at(std::string company_name)
{
    person.company_name = company_name;
    return *this;
}

PersonJobBuilder& PersonJobBuilder::as(std::string position)
{
    person.position = position;
    return *this;
}
