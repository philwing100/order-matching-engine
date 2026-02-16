#include "order_book.h"

#include <chrono>
#include <iostream>
#include <thread>

namespace {
int g_failures = 0;

void expect_true(bool condition, const std::string& message) {
    if (!condition) {
        std::cout << "FAIL: " << message << "\n";
        ++g_failures;
    }
}

void expect_equal_size(size_t actual, size_t expected, const std::string& message) {
    if (actual != expected) {
        std::cout << "FAIL: " << message << " (expected " << expected
                  << ", got " << actual << ")\n";
        ++g_failures;
    }
}

void test_insert_and_delete() {
    OrderBook book("NVDA");
    Order buy(1, "NVDA", true, 10.0, 5.0);

    book.insert_order(buy);
    expect_equal_size(book.order_map.size(), 1, "order_map size after insert");
    expect_equal_size(book.bids.size(), 1, "bids size after insert");

    bool deleted = book.delete_order(1);
    expect_true(deleted, "delete_order returns true");
    expect_equal_size(book.order_map.size(), 0, "order_map size after delete");
    expect_equal_size(book.bids.size(), 0, "bids size after delete");
}

void test_crossing_order_rejected() {
    OrderBook book("NVDA");
    Order buy(1, "NVDA", true, 10.0, 5.0);
    Order sell(2, "NVDA", false, 9.0, 5.0);

    book.insert_order(buy);
    book.insert_order(sell);

    expect_equal_size(book.order_map.size(), 1, "crossing order ignored");
    expect_equal_size(book.asks.size(), 0, "asks remains empty after crossing sell");
}

void test_threaded_delete_before_sell() {
    OrderBook book("NVDA");
    Order buy(1, "NVDA", true, 10.0, 5.0);
    Order sell(2, "NVDA", false, 11.0, 5.0);

    book.insert_order(buy);

    std::thread deleter([&book] {
        book.delete_order(1);
    });
    deleter.join();

    book.insert_order(sell);

    expect_equal_size(book.order_map.size(), 1, "only sell order exists after delete");
    expect_equal_size(book.asks.size(), 1, "asks contains sell order");
    expect_equal_size(book.bids.size(), 0, "bids empty after delete");
}
} // namespace

int main() {
    test_insert_and_delete();
    test_crossing_order_rejected();
    test_threaded_delete_before_sell();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    }

    std::cout << g_failures << " test(s) failed.\n";
    return 1;
}
