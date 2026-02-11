#pragma once
#include "order_book.h"
#include <unordered_map>
#include <string>

using namespace std;

//Just exists to match the symbol to the order book as fast as possible
struct engine{
    unordered_map<string, order_book*> books; //symbol to order book

    
    bool add_order();
};

