//matching_engine.h
// matching_engine.h
#pragma once
#include "order_stream.h"
#include "order.h"
#include <thread>
#include <atomic>
#include <unordered_map>
#include <string>
#include "order_book.h"

class MatchingEngine {
public:
    MatchingEngine(OrderStream<Order>& stream);

    void start();
    void stop();

private:
    void run();
    void process(Order& o);

    unordered_map<string, OrderBook*> book_map;
    OrderStream<Order>& stream_;
    std::atomic<bool> running_;
    std::thread thread_;

    
};


