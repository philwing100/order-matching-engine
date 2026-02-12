//order_book.h
#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
#include "order.h"
#include <unordered_map>
using namespace std;

struct OrderBook{
    string symbol;

    unordered_map<int, order*> order_map; //order id to order object pointer


    
};
