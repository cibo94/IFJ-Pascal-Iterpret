/**
 * \file main.c
 * \author Miroslav Cibulka - xcibul10
 * \brief Hlavny spustaci subor
 */

#include "inc.h"
#include <time.h>

int main (int argc, char ** argv) {
#ifndef __GNUC__
    __init__();
#endif
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
    print_EIP(EIP);
#endif
    INT_interpret();
#ifdef TICK
    end = clock();
    fprintf(stderr,"%u\n", (unsigned)(end-start));
#else
    fclose(f);
#endif
#ifndef __GNUC__
    __free__();
#endif
    return 0;
}
