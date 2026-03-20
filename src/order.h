//order.h
#pragma once

#include <iostream>

class Order {
public:
    Order(int id, int symbol_id, bool side, double price, double quantity);
    Order() = default;

    void print_order();

    // All getters are defined inline so the compiler can fold them away
    // entirely in the hot path without LTO.
    inline int    getId()       const { return id_; }
    inline int    getSymbolId() const { return symbol_id_; }
    inline bool   getSide()     const { return side_; }
    inline double getPrice()    const { return price_; }
    inline double getQuantity() const { return quantity_; }

private:
    // Doubles first: eliminates the 7-byte mid-struct padding that the
    // original {int,int,bool,double,double} layout produced.
    double price_;       // offset  0
    double quantity_;    // offset  8
    int    id_;          // offset 16
    int    symbol_id_;   // offset 20
    bool   side_;        // offset 24
    // 7 bytes trailing pad → sizeof(Order) == 32
};
