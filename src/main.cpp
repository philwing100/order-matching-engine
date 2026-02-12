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

    Exchange exchange(stream, 100000, 100, 10);
    MatchingEngine engine(stream);

    engine.start();
    exchange.start();

    exchange.stop();
    engine.stop();

    return 0;

    cout<<opts.num_orders;
    cout<<"\n after\n";

    Order order(1,true,23.0,5.0, "Test"); 
    order.print_order();

    //Exchange generates orders based on the passed in args

    //Optional 

    //Order book processes the generated args
    
    


    return 0;
}

