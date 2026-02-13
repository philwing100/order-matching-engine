#include <iostream>
#include <cstdlib>
#include <cmath>
#include "./helpers/argparse.h"
#include "order.h"
#include "order_stream.h"
#include "exchange.h"
#include "matching_engine.h"

using namespace std;


int main(int argc, char **argv) {

    struct options_t opts;
    get_opts(argc, argv, &opts);

    OrderStream<Order> stream;

    Exchange exchange(stream, opts.num_orders, opts.symbols, opts.duration);
    //MatchingEngine engine(stream);

   // engine.start();
    exchange.start(2);

    exchange.stop();
    //engine.stop();

    return 0;
}

