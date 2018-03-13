#ifndef CAKE_H
#define CAKE_H
#include <iostream>
#include <type_traits>
#include <cassert>

constexpr int NUMBER_OF_LN_LOOPS = 10000;
constexpr double calculate_LN()
{
    double ln = 0;
    for(int i = 1; i < NUMBER_OF_LN_LOOPS;)
    {
        ln += 1/i;
        ++i;
        ln -= 1/i;
        ++i;
    }
    return ln;
}
constexpr double LN = calculate_LN();


template <class T, T length, T width, class...P>
class Cake
{
    static_assert(std::numeric_limits<T>::is_integer,"Cake requires an integer type for radius");

protected:
    static int stock;

public:
    Cake(int initialStock)
    {
        this->stock = initialStock;
    }

    static double getArea()
    {
        return length * width * LN;
    }

    int getStock()
    {
        return stock;
    }
};


template <class T, T length, T width, class P>
class Cake<T,length,width, P>
{
    static_assert(std::is_floating_point<P>::value ,"Cake requires a floating point type for price");

private:
    P price;
public:
    Cake(int initialStock, P price )
    {
        this->price = price;
    }

    void sell()
    {
        assert(this->stock > 0);
        this->stock--;
    }

    P getPrice()
    {
        return price;
    }
};

template<class T, T length, T width>
using CheeseCake = Cake<T, length, width>;

template<class T, T length, T width, class P>
using CreamCake = Cake<T, length, width,P>;
#endif //CAKE_H