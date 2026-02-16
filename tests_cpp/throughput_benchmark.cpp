#include "../src/order.h"
#include "../src/order_stream.h"
#include "../src/exchange.h"
#include "../src/matching_engine.h"
#include "../src/helpers/argparse.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

namespace {
long read_rss_kb() {
    std::ifstream file("/proc/self/status");
    if (!file.is_open()) {
        return -1;
    }

    std::string key;
    while (file >> key) {
        if (key == "VmRSS:") {
            long value_kb = 0;
            std::string unit;
            file >> value_kb >> unit;
            return value_kb;
        }
        std::string rest;
        std::getline(file, rest);
    }
    return -1;
}
}

int main(int argc, char **argv) {
    options_t opts;
    get_opts(argc, argv, &opts);

    if (opts.num_orders <= 0) {
        std::cout << "num_orders must be > 0\n";
        return 1;
    }
    if (opts.duration <= 0) {
        std::cout << "duration must be > 0\n";
        return 1;
    }
    if (opts.symbols <= 0) {
        std::cout << "symbols must be > 0\n";
        return 1;
    }

    OrderStream<Order> stream;
    Exchange exchange(stream, opts.num_orders, opts.symbols, opts.duration);
    MatchingEngine engine(stream);

    const auto start = std::chrono::steady_clock::now();
    engine.start();
    exchange.start(0);

    exchange.wait();
    engine.stop();

    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = end - start;

    const double seconds = elapsed.count();
    const uint64_t processed = engine.processed_count();
    const uint64_t matched = engine.match_count();
    const double orders_per_sec = seconds > 0.0 ? processed / seconds : 0.0;
    const double matches_per_sec = seconds > 0.0 ? matched / seconds : 0.0;
    const long rss_kb = read_rss_kb();

    std::cout << "Duration(s): " << seconds << "\n";
    std::cout << "Orders processed: " << processed << "\n";
    std::cout << "Matches: " << matched << "\n";
    std::cout << "Orders/sec: " << orders_per_sec << "\n";
    std::cout << "Matches/sec: " << matches_per_sec << "\n";
    if (rss_kb >= 0) {
        std::cout << "RSS(kB): " << rss_kb << "\n";
    } else {
        std::cout << "RSS(kB): unavailable\n";
    }

    return 0;
}
