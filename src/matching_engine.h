//matching_engine.h
#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include "order.h"
#include "order_book.h"
#include "order_stream.h"

class MatchingEngine {
public:
    explicit MatchingEngine(OrderStream<Order>& stream);

    void start();
    void stop();
    bool delete_order(int symbol_id, int order_id);

    uint64_t processed_count() const;
    uint64_t match_count()     const;

private:
    void run();

    OrderBook* lookup_book(int symbol_id);
    OrderBook* create_new_book(int symbol_id);
    bool       insert_order(Order& o);

    // Vector indexed by symbol_id — O(1) lookup (bounds check + array index,
    // no hash), grows on demand so there is no fixed symbol-count limit.
    std::vector<std::unique_ptr<OrderBook>> books_;

    OrderStream<Order>& stream_;
    std::atomic<bool>   running_;

    // Plain integers: only written by the engine thread, only read after
    // thread_.join() in stop(), so atomic RMW is unnecessary.
    uint64_t processed_{0};
    uint64_t matched_{0};

    std::thread thread_;
};
