
//Exchange.cpp
#include "exchange.h"
#include <chrono>
#include <random>

using namespace std;
Exchange::Exchange(OrderStream<Order>& stream,
                   int orders_per_sec,
                   int num_symbols,
                   int duration_sec)
    : stream_(stream),
      rate_(orders_per_sec),
      symbols_(num_symbols),
      duration_(duration_sec),
      running_(false) {}

void Exchange::start(int opt) {
    running_ = true;
    thread_ = std::thread(&Exchange::run, this, opt);
}

void Exchange::stop() {
    running_ = false;
    if (thread_.joinable())
        thread_.join();
}

string symbols[3] = {"NVDA", "AMD", "INTL"};

string generateSymbol(){

}

void addOrderToExchange(){
    
}

void Exchange::run(int opt) {
    int order_id = 0;
    if (opt > symbols->length()){

    }

    for(order_id; order_id<5; order_id++){
        Order order(order_id,"NVDA", 1, 10.0, 10.0);
        order.print_order();
        stream_.push(order);
    }

    running_ = false;
}
