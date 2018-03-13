#ifndef PIE_H
#define PIE_H
#include <iostream>
#include <type_traits>
#include <cassert>

constexpr int NUMBER_OF_PI_LOOPS = 625;
constexpr double calculate_PI()
{
    double pi = 3;
    for(int i = 1; i < NUMBER_OF_PI_LOOPS;)
    {
        pi += (4)/((2*i)*(2*i+1)*(2*i+2));
        ++i;
        pi += (4)/((2*i)*(2*i+1)*(2*i+2));
        ++i;
    }
    return pi;
}
constexpr double PI = calculate_PI();


template <class R, R radius, class...P>
class Pie
{
    static_assert(std::numeric_limits<R>::is_integer,"Pie requires an integer type for radius");

protected:
    int stock;

public:
    Pie(int initialStock)
    {
        this->stock = initialStock;
    }

    static double getArea()
    {
        return radius*radius*PI;
    }

    int getStock()
    {
        return stock;
    }
};


/*template <class R, R radius>
class Pie<R,radius>
{};*/


template <class R, R radius, class P>
class Pie<R, radius, P>
{
    static_assert(std::is_floating_point<P>::value ,"Pie requires a floating point type for price");

private:
    P price;

public:
    Pie(int initialStock, P price) : Pie<R,radius,P>(initialStock)
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

template<class R, R radius>
using CherryPie = Pie<R, radius>;

template<class R, R radius, class P>
using ApplePie = Pie<R, radius, P>;


#endif //PIE_H