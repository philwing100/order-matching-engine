//matching_engine.h
// matching_engine.h
#pragma once
#include "order_stream.h"
#include "order.h"
#include <thread>
#include <atomic>

class MatchingEngine {
public:
    MatchingEngine(OrderStream<Order>& stream);

    void start();
    void stop();

private:
    void run();
    void process(Order& o);

    OrderStream<Order>& stream_;
    std::atomic<bool> running_;
    std::thread thread_;
};


