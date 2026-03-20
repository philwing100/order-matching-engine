//Order.cpp
#include "order.h"

// True  = bid
// False = ask
Order::Order(int id, int symbol_id, bool side, double price, double quantity)
    : price_(price), quantity_(quantity), id_(id), symbol_id_(symbol_id), side_(side) {}

void Order::print_order() {
    std::cout << "Order id "   << id_        << "\n";
    std::cout << "Symbol id "  << symbol_id_ << "\n";
    std::cout << "Side "       << side_      << "\n";
    std::cout << "Price "      << price_     << "\n";
    std::cout << "Quantity "   << quantity_  << "\n";
}
