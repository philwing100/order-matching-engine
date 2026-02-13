//argparse.h
#pragma once

#include <getopt.h>
#include <stdlib.h>
#include <iostream>

struct options_t {
    char *in_file;
    char *out_file;
    int num_orders;
    int duration;
    int symbols;
};

void get_opts(int argc, char **argv, struct options_t *opts);

