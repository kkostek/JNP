#include "person.h"

void Adult::buyDecoration(Decoration& dec) {
    this->savings -= dec.getPrice();
}

void Child::doOperation(std::ostream& os, ChristmasTree* tree) {
    os << "Wow!\n";
    (*(--tree->end()))->doOperation(os);
}

void Adult::doOperation(std::ostream& os, ChristmasTree* tree) {
    os << "We have new decoration!\n";
    os <<"Total cost is " << tree->getPrice() << ".\n";
}
