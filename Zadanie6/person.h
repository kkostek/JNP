#ifndef PERSON_H
#define PERSON_H

#include <iostream>

#ifndef HELPER_H
#include "helper.h"
#endif /* HELPER_H */

#ifndef DECORATION_H
#include "decoration.h"
#endif /* DECORATION_H */

class Decoration;

class Price;

class ChristmasTree;


class Person {

protected:
    std::string name;

public:
    Person(std::string my_name) : name(my_name) {}

    std::string getName() {
        return this->name;
    }

    virtual void doOperation(std::ostream& os, ChristmasTree* tree) = 0;
};


class Adult : public Person {

private:
    Price savings = Price(0);

public:
    Adult(std::string my_name, Price sav) : Person(my_name), savings(sav) {}

    Adult(std::string my_name) : Person(my_name) {}

    void buyDecoration(Decoration& dec);

    Price getSavings() {
        return this->savings;
    }

    void doOperation(std::ostream& os, ChristmasTree* tree) override;
};


class Child : public Person {

public:
    Child(std::string my_name) : Person(my_name) {}

    void doOperation(std::ostream& os, ChristmasTree* tree) override;
};

#endif /* PERSON_H */
