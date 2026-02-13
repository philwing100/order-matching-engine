//Order.h
#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
using namespace std;

class Order{
    public:

    Order(int id, string symbol, bool side, double price, double quantity);

    void print_order();

    private:
        int id_;
        string symbol_;
        bool side_;
        double price_;
        double quantity_;
};

