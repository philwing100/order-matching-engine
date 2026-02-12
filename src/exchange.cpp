
//Exchange.cpp
#include "exchange.h"
#include <chrono>
#include <random>

Exchange::Exchange(OrderStream<Order>& stream,
                   int orders_per_sec,
                   int num_symbols,
                   int duration_sec)
    : stream_(stream),
      rate_(orders_per_sec),
      symbols_(num_symbols),
      duration_(duration_sec),
      running_(false) {}

void Exchange::start() {
    running_ = true;
    thread_ = std::thread(&Exchange::run, this);
}

void Exchange::stop() {
    running_ = false;
    if (thread_.joinable())
        thread_.join();
}

void Exchange::run() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<uint32_t> symbol_dist(0, symbols_ - 1);
    std::uniform_real_distribution<double> price_dist(10.0, 200.0);

    uint64_t order_id = 0;
    auto interval = std::chrono::nanoseconds(1'000'000'000 / rate_);
    auto next_time = std::chrono::steady_clock::now();

    auto end_time = next_time + std::chrono::seconds(duration_);

    while (running_ && next_time < end_time) {
        next_time += interval;

        Order o{
            order_id++,
            symbol_dist(rng),
            price_dist(rng),
            100
        };

        stream_.push(o);
        std::this_thread::sleep_until(next_time);
    }

    running_ = false;
}
