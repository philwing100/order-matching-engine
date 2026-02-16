//order_book.cpp
#include "order_book.h"

OrderBook::OrderBook(string symbol)
        : symbol_(symbol){}

bool OrderBook::insert_order(Order& o){
        //check if this is bid side or ask side and attempt to match before insertion
        const bool side = o.getSide();
        const int price_key = static_cast<int>(o.getPrice());

        if (side) {
                // Bid crosses the best ask
                if (!asks.empty() && price_key >= asks.begin()->first) {
                        return true;
                }
        } else {
                // Ask crosses the best bid
                if (!bids.empty() && price_key <= bids.rbegin()->first) {
                        return true;
                }
        }

        //if no match or only partial fill then insert at the price level and in the order map
        Order* order_ptr = new Order(o);

        //insert at price level
        insert_price_level(side, order_ptr);
        //insert in order map
        insert_into_order_map(order_ptr);

        return false;
}

void OrderBook::insert_price_level(const bool side, Order* order){
        //if a pricelevel for that orders price exists, then insert it
        // if the pricelevel does not exist then create it
        std::map<int, PriceLevel>& book = side ? bids : asks;
        const int price_key = static_cast<int>(order->getPrice());
        auto it = book.find(price_key);
        if (it == book.end()) {
                book.emplace(price_key, create_price_level(order));
                return;
        }

        PriceLevel& level = it->second;
        level.total_quantity += static_cast<int>(order->getQuantity());
        if (!level.head) {
                level.head = order;
                level.tail = order;
                return;
        }

        level.tail = order;
}

PriceLevel OrderBook::create_price_level(Order* order){
        PriceLevel level;
        level.total_quantity = static_cast<int>(order->getQuantity());
        level.head = order;
        level.tail = order;
        return level;
}

void OrderBook::insert_into_order_map(Order* order){
        order_map[order->getId()] = order;
}

Order* OrderBook::get_from_order_map(const int order_id){
        auto it = order_map.find(order_id);
        if (it == order_map.end()) {
                return nullptr;
        }
        return it->second;
}

bool OrderBook::delete_from_order_map(const int order_id){
        auto it = order_map.find(order_id);
        if (it == order_map.end()) {
                return false;
        }
        delete it->second;
        order_map.erase(it);
        return true;
}

bool OrderBook::delete_order(const int order_id){
        Order* order = get_from_order_map(order_id);
        if (!order) {
                return false;
        }

        remove_from_price_level(order, order->getSide());
        return delete_from_order_map(order_id);
}

void OrderBook::remove_from_price_level(Order* order, const bool side){
        std::map<int, PriceLevel>& book = side ? bids : asks;
        const int price_key = static_cast<int>(order->getPrice());
        auto it = book.find(price_key);
        if (it == book.end()) {
                return;
        }

        PriceLevel& level = it->second;
        level.total_quantity -= static_cast<int>(order->getQuantity());
        if (level.head == order) {
                level.head = nullptr;
        }
        if (level.tail == order) {
                level.tail = nullptr;
        }

        if (level.total_quantity <= 0 || (!level.head && !level.tail)) {
                book.erase(it);
        }
}





