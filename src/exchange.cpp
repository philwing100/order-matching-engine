
//Exchange.cpp
#include "exchange.h"
#include <chrono>
#include <random>
#include <thread>
#include <vector>

using namespace std;
Exchange::Exchange(OrderStream<Order>& stream,
                   int orders_per_sec,
                   int num_symbols,
                   int duration_sec)
    : stream_(stream),
      rate_(orders_per_sec),
      symbols_(num_symbols),
      duration_(duration_sec),
      running_(false) {}

void Exchange::start(int opt) {
    running_ = true;
    thread_ = std::thread(&Exchange::run, this, opt);
}

void Exchange::stop() {
    running_ = false;
    if (thread_.joinable())
        thread_.join();
}

void Exchange::wait() {
    if (thread_.joinable())
        thread_.join();
}

void Exchange::run(int opt) {
    (void)opt;
    const int symbol_count = symbols_ > 0 ? symbols_ : 1;
    const int duration_sec = duration_ > 0 ? duration_ : 1;

    std::vector<std::string> symbol_list;
    symbol_list.reserve(symbol_count);
    const std::string base_symbols[3] = {"NVDA", "AMD", "INTL"};
    for (int i = 0; i < symbol_count; ++i) {
        if (i < 3) {
            symbol_list.push_back(base_symbols[i]);
        } else {
            symbol_list.push_back("SYM" + std::to_string(i));
        }
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> symbol_pick(0, symbol_count - 1);
    std::uniform_real_distribution<double> mid_dist(50.0, 150.0);
    std::uniform_int_distribution<int> qty_dist(1, 100);
    std::bernoulli_distribution side_dist(0.5);

    std::vector<double> mids(symbol_count);
    for (int i = 0; i < symbol_count; ++i) {
        mids[i] = mid_dist(rng);
    }

    int order_id = 0;
    const auto start = std::chrono::steady_clock::now();
    const auto end = start + std::chrono::seconds(duration_sec);

    while (running_ && std::chrono::steady_clock::now() < end) {
        const int batch = rate_ > 0 ? rate_ : 1;
        for (int i = 0; i < batch; ++i) {
            if (!running_ || std::chrono::steady_clock::now() >= end) {
                break;
            }
            const int idx = symbol_pick(rng);
            const double mid = mids[idx];
            std::normal_distribution<double> price_dist(0.0, mid * 0.01);
            double price = mid + price_dist(rng);
            if (price < 0.01) {
                price = 0.01;
            }
            const bool side = side_dist(rng);
            const double qty = static_cast<double>(qty_dist(rng));

            Order order(order_id++, symbol_list[idx], side, price, qty);
            stream_.push(order);
        }

        if (rate_ > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    running_ = false;
}
