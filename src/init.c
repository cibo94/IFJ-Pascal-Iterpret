#include "inc.h"

char * FILE_NAME;

FILE *init(const char *fn) {
    FILE *ret = fopen(fn, "r");
    size_t size = sizeof(char) * strlen(fn) + 1;
    if (!(FILE_NAME = malloc(size)) || !ret)
        error(ERR_INTERNAL, "Chyba pri otvarani suboru");
    memcpy(FILE_NAME, fn, size);
    return ret;
}
void  interpret() {

}
constructor static void __init__ (void) {
   // log("Initialization");
}
destructor static void __free__ (void) {
    free(FILE_NAME);
    FILE_NAME = NULL;
    //log("Destruction");
}