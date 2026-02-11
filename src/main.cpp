#include <iostream>
#include <cstdlib>
#include <cmath>
#include "./helpers/argparse.h"
#include "order.h"

using namespace std;


int main(int argc, char **argv) {

    cout<< "Hello, World!\n";

    //Read in args for the exchange
    struct options_t opts;
    get_opts(argc, argv, &opts);

    cout<<opts.num_orders;
    cout<<"\n after\n";

    Order order(1,true,23.0,5.0, "Test");
    order.print_order();

    //Exchange generates orders based on the passed in args

    //Optional 

    //Order book processes the generated args
    
    


    return 0;
}

