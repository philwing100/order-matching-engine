#include <argparse.h>

void get_opts(int argc,
              char **argv,
              struct options_t *opts)
{
    if (argc == 1)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t--in or -i <file_path>" << std::endl;
        std::cout << "\t--out or -o <file_path>" << std::endl;
        std::cout << "\t--num_orders or -n <num_orders>" << std::endl;
        std::cout << "\t--duration or -l <num_duration>" << std::endl;
        exit(0);
    }

    struct option l_opts[] = {
        {"in", required_argument, NULL, 'i'},
        {"out", required_argument, NULL, 'o'},
        {"num_orders", required_argument, NULL, 'n'},
        {"duration", required_argument, NULL, 'l'}
    };

    int ind, c;
    while ((c = getopt_long(argc, argv, "i:o:n:sl:", l_opts, &ind)) != -1)
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
        case ':':
            std::cerr << argv[0] << ": option -" << (char)optopt << "requires an argument." << std::endl;
            exit(1);
        }
    }
}
