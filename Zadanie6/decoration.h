#ifndef DECORATION_H
#define DECORATION_H

//#include <cmath>
#include <vector>
//#include <iterator>
#include <memory>
#include "helper.h"

#ifndef PERSON_H
#include "person.h"
#endif

class Person;


class Decoration {
    
protected:
    std::string name;
    Price price = Price(0);
    
public:
    Decoration(std::string name) : name(name) {}
    
    Decoration(std::string name, Price price) : name(name), price(price) {}

    Price getPrice() {
        return price;
    }
    
    std::string getName() {
        return name;
    }
    
    virtual void doOperation(std::ostream& os) = 0;
};


class GlassBall : public Decoration {
private:
    bool broken;
    
public:
    GlassBall(std::string name, Price price) : Decoration(name, price), broken(false) {}
    
    void doOperation(std::ostream& os) override;
};
        

class Lights : public Decoration {

private:
    int mode;
    
public:
    Lights(std::string name, Price price) : Decoration(name, price), mode(1) {}

    void doOperation(std::ostream& os) override;
};


class Tree : public Decoration {

private:
    Date cutDate;
    
public:
    Tree(std::string name, Price price, Date cutDate) : Decoration(name, price), cutDate(cutDate) {}

    void doOperation(std::ostream& os) override;
};
        
        
class ChristmasTree : public Decoration {

private:
    std::vector<std::shared_ptr<Decoration>> decorations;
    std::vector<std::shared_ptr<Person>> observers;
    
public:
    ChristmasTree(std::string name) : Decoration(name, Price(0)) {}

    using ob_iterator = std::vector<std::shared_ptr<Person>>::iterator;
    using dec_iterator = std::vector<std::shared_ptr<Decoration>>::iterator;

    void addDecoration(std::shared_ptr<Decoration> element);
    
    void removeDecoration(std::vector<std::shared_ptr<Decoration>>::iterator& it);

    void doOperation(std::ostream& os) override;

    void attachObserver(std::shared_ptr<Person> person);

    void detachObserver(ob_iterator& it);

    dec_iterator begin() {
        return decorations.begin();
    }

    dec_iterator end() {
        return decorations.end();
    }

    ob_iterator ob_begin() {
        return observers.begin();
    }

    ob_iterator ob_end() {
        return observers.end();
    }

    void notify();
};

#endif /* DECORATION_H */
