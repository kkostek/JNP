#ifndef __SEJF_H__
#define __SEJF_H__

#include <iostream>
#include "kontroler.h"
using namespace std;

class Sejf {

    friend class Kontroler;

private:
    string napis;
    int dlugosc;
    int liczba_dostepow;
    bool czy_uzyty_modyfikator;

public:
    Sejf(string, int);
    Sejf(string);
    Sejf(Sejf&&);
    Sejf() = delete;
    Sejf(Sejf&) = delete;

    Sejf& operator= (Sejf&) = delete;
    Sejf& operator= (Sejf&&);
    Sejf& operator*= (int);
    Sejf& operator+= (int);
    Sejf& operator-= (int);
    short int operator[] (int);

    Kontroler& kontroler();

};


#endif /* __SEJF_H__ */
