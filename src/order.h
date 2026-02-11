#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
using namespace std;


struct PriceLevel {
    int total_quantity;
    Order* head;
    Order* tail;
};

class Order{
    public:


    Order(int id, string symbol, bool side, PriceLevel price, double quantity, string type);

    void print_order();

    private:
        int id_;
        string symbol_;
        bool side_;
        PriceLevel price_;
        double quantity_;
        double timestamp_;
        string type_;
};
