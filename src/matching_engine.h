//matching_engine.h
// matching_engine.h
#pragma once
#include "order_stream.h"
#include "order.h"
#include <thread>
#include <atomic>
#include <unordered_map>
#include <string>
#include <cstdint>
#include "order_book.h"

class MatchingEngine {
public:
    MatchingEngine(OrderStream<Order>& stream);

    void start();
    void stop();
    bool delete_order(const std::string& symbol, const int order_id);
    uint64_t processed_count() const;
    uint64_t match_count() const;
 
private:
    void run();
    void process(Order& o);
    OrderBook* lookup_book(const std::string& symbol);
    bool create_new_book(const std::string& symbol);
    bool insert_order(Order& o);
    

    std::unordered_map<std::string, OrderBook> book_map;
    OrderStream<Order>& stream_;
    std::atomic<bool> running_;
    std::atomic<uint64_t> processed_;
    std::atomic<uint64_t> matched_;
    std::thread thread_;
};


