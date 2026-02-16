//matching_engine.cpp
#include "matching_engine.h"

MatchingEngine::MatchingEngine(OrderStream<Order>& stream)
    : stream_(stream), running_(false), processed_(0), matched_(0){}

void MatchingEngine::start(){
    running_ = true;
    thread_ = std::thread(&MatchingEngine::run, this);
}

void MatchingEngine::stop(){
    running_ = false;
    stream_.push(Order(-1, "", true, 0.0, 0.0));
    if (thread_.joinable())
        thread_.join();
}

bool MatchingEngine::insert_order(Order& o){
    std::string symbol = o.getSymbol();
    OrderBook* ref = lookup_book(symbol);
    if (ref) {
        return ref->insert_order(o);
    }

    if (create_new_book(symbol)) {
        ref = lookup_book(symbol);
        if (ref) {
            return ref->insert_order(o);
        }
    }

    return false;
}

bool MatchingEngine::create_new_book(const std::string& symbol){
    auto [it, inserted] = book_map.emplace(symbol, OrderBook(symbol));
    return inserted;
}

OrderBook* MatchingEngine::lookup_book(const std::string& symbol){
    auto it = book_map.find(symbol);
    return (it != book_map.end()) ? &it->second : nullptr;
}

bool MatchingEngine::delete_order(const std::string& symbol, const int order_id){
    OrderBook* ref = lookup_book(symbol);
    if (!ref) {
        return false;
    }
    return ref->delete_order(order_id);
}

void MatchingEngine::run() {
    while (running_) {
        Order o = stream_.pop();
        if (o.getId() < 0) {
            break;
        }
        ++processed_;
        if (insert_order(o)) {
            ++matched_;
        }
    }
    running_ = false;
}

uint64_t MatchingEngine::processed_count() const {
    return processed_.load();
}

uint64_t MatchingEngine::match_count() const {
    return matched_.load();
}

