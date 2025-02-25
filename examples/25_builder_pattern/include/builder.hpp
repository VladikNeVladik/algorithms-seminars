// Copyright 2025 Sergei Afonin & Vladislav Aleinik
#ifndef HEADER_GUARD_BUILDER_HPP_INCLUDED
#define HEADER_GUARD_BUILDER_HPP_INCLUDED

#include <string>
#include <iostream>

class PersonBuilder;
class PersonAddressBuilder;
class PersonJobBuilder;

class Person {
public:
    Person() = default;

    static PersonBuilder create();

    friend std::ostream& operator<<(std::ostream& os, const Person& p);

    std::string street_address;
    std::string post_code;
    std::string city;

    std::string company_name;
    std::string position;
    int annual_income = 0;
};

class PersonBuilderBase {
public:
    explicit PersonBuilderBase(Person& person);

    operator Person();

    PersonAddressBuilder lives() const;
    PersonJobBuilder works() const;

protected:
    Person& person;
};

class PersonBuilder : public PersonBuilderBase
{
public:
    PersonBuilder();
private:
    Person p;
};

class PersonAddressBuilder : public PersonBuilderBase
{
public:
    explicit PersonAddressBuilder(Person & person);

    PersonAddressBuilder& city(std::string city);

    PersonAddressBuilder& at(std::string street_address);
};


class PersonJobBuilder : public PersonBuilderBase
{
public:
    explicit PersonJobBuilder(Person & person);

    PersonJobBuilder& at(std::string company_name);

    PersonJobBuilder& as(std::string position);
};

#endif // HEADER_GUARD_BUILDER_HPP_INCLUDED
