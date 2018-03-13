#ifndef BAKERY_H
#define BAKERY_H
#include <cassert>
#include <limits>
#include <tuple>
#include <set>
#include <typeinfo>
#include <type_traits>
using namespace std;

template <class C, class A, A shelfArea, class... P>
class Bakery {
    C profits = 0;
    tuple<P...> prod;
    static tuple<P...> classes = {P...};
    set<type_info> types;
    bool checkTypes() {
        for (auto a : this->classes) {
            enable_if<this->types.find(typeid(a)) != this->types.end()>::type;
            this->types.insert(typeid(a));
        }
        return true;
    }

    static_assert(checkTypes(), "xD");
    static_assert(numeric_limits<C>::is_floating_point, "C class is not a floating_point");
    static_assert(numeric_limits<A>::is_integer, "A is not a integer");

public:
    Bakery(P&&... products) {
        prod(products...);
    }

    C getProfits() {
        return this->profits;
    }
/*
    template <class Product>
    void sell() {
        enable_if<is_void<Product.sell()>::value, Product>::type;
        enable_if<types.find(Product) != types.end(), Product>::type;
        //dla tupli inaczej
        //tuple<>.get
    }*/

    template <class Product>
    int getProductStock() {
        enable_if<types.find(typeid(Product)) != types.end(), Product>::type;
        for (auto product : products) {
            if (typeid(product) == typeid(Product)) {
                product.getStock();
                return;
            }
        }
    }

    template <class Product>
    void restock(int additionalStock) {
        template enable_if<is_same<decltype(Product.restock()), void>>::type;
        template enable_if< types.find(Product) != types.end(), Product >::type;
        //TODO ApplePie should have method restock() ;)
    }
};

#endif //BAKERY_H
