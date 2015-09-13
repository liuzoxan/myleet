#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int nflag = 0;
    int seconds = 0, tfnd = 0;

    int opt = -1;
    while((opt = getopt(argc, argv, "nt:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            nflag = 1;
            break;
        case 't':
            seconds = atoi(optarg);
            tfnd = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    fprintf(stdout, "nflag=%d tfnd=%d optind=%d\n", nflag, tfnd, optind);

    if (optind >= argc)
    {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_SUCCESS);
    }

    if (tfnd)
    {
        fprintf(stdout, "lord, gonna sleep %d secs\n", seconds);
        sleep(seconds);
    }
    fprintf(stdout, "name argument: %s\n", argv[optind]);

    return EXIT_SUCCESS;
}

