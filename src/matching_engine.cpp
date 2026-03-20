//matching_engine.cpp
#include "matching_engine.h"

MatchingEngine::MatchingEngine(OrderStream<Order>& stream)
    : stream_(stream), running_(false) {}

void MatchingEngine::start() {
    running_ = true;
    thread_  = std::thread(&MatchingEngine::run, this);
}

void MatchingEngine::stop() {
    running_ = false;
    // Unblock the consumer spin with a sentinel (id < 0).
    stream_.push(Order(-1, -1, true, 0.0, 0.0));
    if (thread_.joinable()) {
        thread_.join();
        // join() provides a happens-before edge: processed_ and matched_
        // written by the engine thread are visible here without atomics.
    }
}

bool MatchingEngine::insert_order(Order& o) {
    const int symbol_id = o.getSymbolId();
    OrderBook* ref = lookup_book(symbol_id);
    if (!ref) {
        ref = create_new_book(symbol_id);
    }
    return ref ? ref->insert_order(o) : false;
}

// Returns the new book pointer so insert_order never re-indexes books_ after
// a resize (the resize may reallocate the internal buffer).
OrderBook* MatchingEngine::create_new_book(int symbol_id) {
    if (symbol_id < 0) return nullptr;
    if (symbol_id >= static_cast<int>(books_.size())) {
        books_.resize(symbol_id + 1);
    }
    books_[symbol_id] = std::make_unique<OrderBook>(symbol_id);
    return books_[symbol_id].get();
}

OrderBook* MatchingEngine::lookup_book(int symbol_id) {
    if (symbol_id < 0 || symbol_id >= static_cast<int>(books_.size())) {
        return nullptr;
    }
    return books_[symbol_id].get();
}

bool MatchingEngine::delete_order(int symbol_id, int order_id) {
    OrderBook* ref = lookup_book(symbol_id);
    return ref ? ref->delete_order(order_id) : false;
}

void MatchingEngine::run() {
    // Loop exits only via the sentinel (id < 0) pushed by stop().
    // Removing the running_ check from here eliminates one atomic load per
    // order in the hot path.
    for (;;) {
        Order o = stream_.pop();
        if (o.getId() < 0) break;
        ++processed_;
        if (insert_order(o)) ++matched_;
    }
}

uint64_t MatchingEngine::processed_count() const { return processed_; }
uint64_t MatchingEngine::match_count()     const { return matched_; }
