//order_book.cpp
#include "order_book.h"

#include <climits>

OrderBook::OrderBook(int symbol_id)
    : symbol_id_(symbol_id) {}

OrderBook::~OrderBook() = default;

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

bool OrderBook::insert_order(Order& o) {
    const bool side      = o.getSide();
    const int  price_key = static_cast<int>(o.getPrice());  // computed once

    if (side) {
        if (!asks.empty() && price_key >= best_ask_) {
            return true;
        }
    } else {
        if (!bids.empty() && price_key <= best_bid_) {
            return true;
        }
    }

    Order* order_ptr = pool_.alloc();
    *order_ptr = o;

    insert_price_level(side, order_ptr, price_key);  // price_key forwarded
    insert_into_order_map(order_ptr);
    return false;
}

bool OrderBook::delete_order(int order_id) {
    Order* order = get_from_order_map(order_id);
    if (!order) {
        return false;
    }
    remove_from_price_level(order, order->getSide());
    return delete_from_order_map(order_id);
}

// ---------------------------------------------------------------------------
// Price-level helpers
// ---------------------------------------------------------------------------

void OrderBook::insert_price_level(bool side, Order* order, int price_key) {
    std::unordered_map<int, PriceLevel>& book = side ? bids : asks;

    auto it = book.find(price_key);
    if (it == book.end()) {
        book.emplace(price_key, create_price_level(order));
    } else {
        PriceLevel& level = it->second;
        level.total_quantity += static_cast<int>(order->getQuantity());
        if (!level.head) {
            level.head = order;
            level.tail = order;
        } else {
            level.tail = order;
        }
    }

    if (side) {
        if (price_key > best_bid_) best_bid_ = price_key;
    } else {
        if (price_key < best_ask_) best_ask_ = price_key;
    }
}

PriceLevel OrderBook::create_price_level(Order* order) {
    return {static_cast<int>(order->getQuantity()), order, order};
}

void OrderBook::remove_from_price_level(Order* order, bool side) {
    std::unordered_map<int, PriceLevel>& book = side ? bids : asks;
    const int price_key = static_cast<int>(order->getPrice());

    auto it = book.find(price_key);
    if (it == book.end()) return;

    PriceLevel& level = it->second;
    level.total_quantity -= static_cast<int>(order->getQuantity());
    if (level.head == order) level.head = nullptr;
    if (level.tail == order) level.tail = nullptr;

    if (level.total_quantity <= 0 || (!level.head && !level.tail)) {
        book.erase(it);
        if (side) {
            if (price_key == best_bid_) update_best_bid();
        } else {
            if (price_key == best_ask_) update_best_ask();
        }
    }
}

// ---------------------------------------------------------------------------
// Order-map helpers — dense vector indexed by per-book order ID
// ---------------------------------------------------------------------------

void OrderBook::insert_into_order_map(Order* order) {
    const int id = order->getId();
    if (id >= static_cast<int>(order_map.size())) {
        order_map.resize(id + 1, nullptr);
    }
    order_map[id] = order;
    ++live_count_;
}

Order* OrderBook::get_from_order_map(int order_id) {
    if (order_id < 0 || order_id >= static_cast<int>(order_map.size())) {
        return nullptr;
    }
    return order_map[order_id];
}

bool OrderBook::delete_from_order_map(int order_id) {
    if (order_id < 0 || order_id >= static_cast<int>(order_map.size())) {
        return false;
    }
    Order* p = order_map[order_id];
    if (!p) return false;

    pool_.dealloc(p);
    order_map[order_id] = nullptr;
    --live_count_;
    return true;
}

// ---------------------------------------------------------------------------
// Best-price rescans
// ---------------------------------------------------------------------------

void OrderBook::update_best_bid() {
    best_bid_ = INT_MIN;
    for (const auto& [k, _] : bids) {
        if (k > best_bid_) best_bid_ = k;
    }
}

void OrderBook::update_best_ask() {
    best_ask_ = INT_MAX;
    for (const auto& [k, _] : asks) {
        if (k < best_ask_) best_ask_ = k;
    }
}
