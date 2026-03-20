//order_book.h
#pragma once

#include <climits>
#include <unordered_map>
#include <vector>
#include "order.h"
#include "pool.h"

struct PriceLevel {
    int    total_quantity;
    Order* head;
    Order* tail;
};

struct OrderBook {
    explicit OrderBook(int symbol_id);
    ~OrderBook();

    OrderBook(const OrderBook&)            = delete;
    OrderBook& operator=(const OrderBook&) = delete;
    OrderBook(OrderBook&&)                 = default;
    OrderBook& operator=(OrderBook&&)      = default;

    int symbol_id_;

    // Dense vector indexed by per-book order ID.
    // Replaces unordered_map<int,Order*>: zero hash, zero per-entry heap
    // allocation, O(1) insert and lookup via direct array index.
    // Requires the Exchange to assign sequential IDs per symbol.
    std::vector<Order*> order_map;

    // Returns the number of live (non-null) entries.
    size_t order_count() const { return live_count_; }

    std::unordered_map<int, PriceLevel> bids;
    std::unordered_map<int, PriceLevel> asks;

    bool insert_order(Order& o);
    bool delete_order(int order_id);

private:
    // price_key is passed in from insert_order to avoid a second double→int
    // cast inside insert_price_level.
    void   insert_price_level(bool side, Order* order, int price_key);
    PriceLevel create_price_level(Order* order);
    void   insert_into_order_map(Order* order);
    Order* get_from_order_map(int order_id);
    bool   delete_from_order_map(int order_id);
    void   remove_from_price_level(Order* order, bool side);

    int best_bid_{INT_MIN};
    int best_ask_{INT_MAX};

    void update_best_bid();
    void update_best_ask();

    size_t live_count_{0};
    Pool<Order> pool_;
};
