#include "inc.h"
#include <time.h>

int main (int argc, char ** argv) {
#ifndef TICK
    if (argc != 2) error(ERR_INTERNAL, "malo argumentov");
    FILE *f = init(argv[argc-1]);
#else
    FILE *f = stdin;
    stdout  = fopen("/dev/null", "w");
    clock_t start, end;
    start = clock();
#endif
    SYN_parser(f);
#ifdef DEBUG
//    print_EIP(EIP);
#endif
    INT_interpret();
#ifdef TICK
    end = clock();
    fprintf(stderr,"%u\n", (unsigned)(end-start));
#else
    fclose(f);
#endif
    return 0;
}
