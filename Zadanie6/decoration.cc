#include "decoration.h"

void GlassBall::doOperation(std::ostream& os) {
    os << name << ": ";
    if (!broken) {
        int random = randomNumber();
        if (random % 4 == 0) {
            os << "broken\n";
            broken = true;
        }
        else {
            os << "OK\n";
        }
    }
    else {
        os << "broken\n";
    }
}

void Lights::doOperation(std::ostream& os) {
    os << name << ": ";

    if (mode == 0) {
        os << "off\n";
    }
    else if (mode == 1) {
        os << "constant\n";
    }
    else {
        os << "blinking\n";
    }
    mode++;
    mode %= 3;
}

void Tree::doOperation(std::ostream& os) {
    int minutesFromCut = currentDate() - cutDate;
    os << name << ": " << std::max(0, 100 - (int) (minutesFromCut*0.01)) << "% of tree covered with needles\n";
}

void ChristmasTree::addDecoration(std::shared_ptr<Decoration> element) {
    decorations.push_back(element);
    price = price + element->getPrice();
    notify();
}

void ChristmasTree::doOperation(std::ostream& os) {\

    for (auto decor : decorations) {
        decor->doOperation(os);
    }
}

void ChristmasTree::attachObserver(std::shared_ptr<Person> person) {
    observers.push_back(person);
}

void ChristmasTree::detachObserver(ob_iterator& it) {
    observers.erase(it);
}

void ChristmasTree::removeDecoration(ChristmasTree::dec_iterator& it) {
    decorations.erase(it);
}

void ChristmasTree::notify() {

    for (auto person : observers) {
        person->doOperation(std::cout, this);
    }
}
