#include <iostream>
#include <utility>
#include <cassert>
#include "sejf.h"
#define DEFAULT_SIZE 42
using namespace std;

Kontroler& Sejf::kontroler() {
    return Kontroler(&this);
}

Sejf::Sejf(string napis, int liczba_dostepow) {
    this->napis = napis;
    this->dlugosc = napis.size();
    this->liczba_dostepow = liczba_dostepow;
    this->czy_uzyty_modyfikator = false;
}

Sejf::Sejf(string napis) {
    this->napis = napis;
    this->dlugosc = napis.size();
    this->liczba_dostepow = DEFAULT_SIZE;
    this->czy_uzyty_modyfikator = false;
}

Sejf::Sejf(Sejf&& sejf) {
    this->napis = sejf.napis;
    this->dlugosc = sejf.dlugosc;
    this->liczba_dostepow = sejf.liczba_dostepow;
    this->czy_uzyty_modyfikator = sejf.czy_uzyty_modyfikator;
}

Sejf& Sejf::operator= (Sejf&& sejf) {
    this->napis = sejf.napis;
    this->dlugosc = sejf.dlugosc;
    this->liczba_dostepow = sejf.liczba_dostepow;
    this->czy_uzyty_modyfikator = sejf.czy_uzyty_modyfikator;
    return *this;
}

Sejf& Sejf::operator*= (int k) {
    assert (k >= 0);
    this->liczba_dostepow *= k;
    this->czy_uzyty_modyfikator = true;
    return *this;
}

Sejf& Sejf::operator+= (int k) {
    assert (k >= 0);
    this->liczba_dostepow += k;
    this->czy_uzyty_modyfikator = true;
    return *this;
}

Sejf& Sejf::operator-= (int k) {
    assert (k >= 0);
    if (this->liczba_dostepow - k >= 0) {
        this->liczba_dostepow -= k;
        this->czy_uzyty_modyfikator = true;
    }
    return *this;
}

short int Sejf::operator[] (int k) {
    if (k >= 0 && k < this->dlugosc && this->liczba_dostepow > 0) {
        this->liczba_dostepow--;
        return (short int) this->napis[k];
    }
    else return -1;
}
