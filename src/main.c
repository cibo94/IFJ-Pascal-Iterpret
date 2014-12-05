#include "inc.h"

int main (int argc, char ** argv) {
    if (argc != 2) error(ERR_INTERNAL, "malo argumentov");
    FILE *f = init(argv[argc-1]);
    SYN_parser(f);
 //   print_EIP(EIP);
    INT_interpret();
    return 0;
}
