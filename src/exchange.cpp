//Exchange.cpp
#include "exchange.h"

#include <chrono>
#include <random>
#include <thread>
#include <vector>

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
    thread_  = std::thread(&Exchange::run, this, opt);
}

void Exchange::stop() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
}

void Exchange::wait() {
    if (thread_.joinable()) thread_.join();
}

void Exchange::run(int opt) {
    (void)opt;
    const int symbol_count = symbols_ > 0 ? symbols_ : 1;
    const int duration_sec = duration_ > 0 ? duration_ : 1;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int>  symbol_pick(0, symbol_count - 1);
    std::uniform_real_distribution<double> mid_dist(50.0, 150.0);
    std::uniform_int_distribution<int>  qty_dist(1, 100);
    std::bernoulli_distribution         side_dist(0.5);

    std::vector<double> mids(symbol_count);
    for (int i = 0; i < symbol_count; ++i) {
        mids[i] = mid_dist(rng);
    }

    // Pre-build one normal_distribution per symbol so we don't reconstruct
    // it on every order (construction computes sigma and other parameters).
    std::vector<std::normal_distribution<double>> price_dists;
    price_dists.reserve(symbol_count);
    for (int i = 0; i < symbol_count; ++i) {
        price_dists.emplace_back(0.0, mids[i] * 0.01);
    }

    // Per-book sequential order IDs so each OrderBook's order_map vector
    // stays dense and bounded to the number of orders for that symbol.
    std::vector<int> next_id(symbol_count, 0);

    const auto start = std::chrono::steady_clock::now();
    const auto end   = start + std::chrono::seconds(duration_sec);

    while (running_ && std::chrono::steady_clock::now() < end) {
        const int batch = rate_ > 0 ? rate_ : 1;
        for (int i = 0; i < batch; ++i) {
            // Time is checked only in the outer loop (once per batch / second)
            // rather than on every order, eliminating ~batch vDSO calls per sec.
            const int    symbol_id = symbol_pick(rng);
            const double mid       = mids[symbol_id];
            double price = mid + price_dists[symbol_id](rng);
            if (price < 0.01) price = 0.01;

            const bool   side = side_dist(rng);
            const double qty  = static_cast<double>(qty_dist(rng));

            stream_.push(Order(next_id[symbol_id]++, symbol_id, side, price, qty));
        }

        if (rate_ > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    running_ = false;
}
