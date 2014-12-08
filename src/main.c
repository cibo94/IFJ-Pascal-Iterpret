#include "inc.h"
#include <time.h>

int main (int argc, char ** argv) {
    if (argc != 2) error(ERR_INTERNAL, "malo argumentov");
    FILE *f = init(argv[argc-1]);
    clock_t start, end;
    start = clock();
    SYN_parser(f);
//    print_EIP(EIP);
    INT_interpret();
    end = clock();
    printf("%u\n", (unsigned)(end-start));
    fclose(f);
    return 0;
}
