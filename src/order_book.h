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

    void insert_order();
    void delete_order();
};

