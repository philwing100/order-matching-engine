//order_book.h
#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
#include "order.h"
#include <unordered_map>
#include <map>
using namespace std;


struct PriceLevel {
    int total_quantity; 
    Order* head;
    Order* tail;
};

//exists as 
struct OrderBook{
    OrderBook(string symbol);

    string symbol_;

    unordered_map<int, Order*> order_map; //order id to order object pointer
    std::map<int, PriceLevel> bids;
    std::map<int, PriceLevel> asks;

    bool insert_order(Order& o);
    bool delete_order(const int order_id);

    private:
        void insert_price_level(const bool side, Order* order);
        PriceLevel create_price_level(Order* order);
        void insert_into_order_map(Order* order);
        Order* get_from_order_map(const int order_id);
        bool delete_from_order_map(const int order_id);
        void remove_from_price_level(Order* order, const bool side);
};

