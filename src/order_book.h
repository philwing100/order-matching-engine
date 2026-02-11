#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
#include "order.h"
#include <unordered_map>
using namespace std;

struct order_book{
    string symbol;

    unordered_map<int, order*> order_map; //order id to order object pointer


    
};
