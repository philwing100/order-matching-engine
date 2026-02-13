//Argparse.cpp
#include "argparse.h"

void get_opts(int argc,
              char **argv,
              struct options_t *opts)
{
    opts->in_file = nullptr;
opts->out_file = nullptr;
opts->num_orders = 0;
opts->duration = 0;
opts->symbols = 0;
    if (argc == 1)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t--in or -i <file_path>" << std::endl;
        std::cout << "\t--out or -o <file_path>" << std::endl;
        std::cout << "\t--num_orders or -n <number of orders per second>" << std::endl;
        std::cout << "\t--duration or -d <num_duration>" << std::endl;
        std::cout << "\t--num_symbols or -s <number of symbols used>" << std::endl;
        exit(0);
    }

    struct option l_opts[] = {
        {"in", false, NULL, 'i'},
        {"out", false, NULL, 'o'},
        {"num_orders", required_argument, NULL, 'n'},
        {"duration", required_argument, NULL, 'd'},
        {"symbols", required_argument, NULL, 's'}
    };

    int ind, c;
    while ((c = getopt_long(argc, argv, "i:o:n:d:s:", l_opts, &ind)) != -1)
    {
        switch (c)
        {
        case 0:
            break;
        case 'i':
            opts->in_file = (char *)optarg;
            break;
        case 'o':
            opts->out_file = (char *)optarg;
            break;
        case 'n':
            opts->num_orders = atoi((char *)optarg);
            break;
        case 'd':
            opts->duration = atoi((char *)optarg);
            break;
        case 's':
            opts->symbols = atoi((char *)optarg);
            break;
        case ':':
            std::cerr << argv[0] << ": option -" << (char)optopt << "requires an argument." << std::endl;
            exit(1);
        }
    }
}
