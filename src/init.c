#include "inc.h"

char * FILE_NAME;
PGLOB_DEST pointers;

FILE *init(const char *fn) {
    FILE *ret = fopen(fn, "r");
    size_t size = sizeof(char) * strlen(fn) + 1;
    if (!(FILE_NAME = malloc(size)) || !ret)
        error(ERR_INTERNAL, "Chyba pri otvarani suboru");
    memcpy(FILE_NAME, fn, size);
    return ret;
}
void  interpret() {
    return;
}
constructor static void __init__ (void) {
    pointers = malloc (sizeof(GLOB_DEST));
    if (pointers == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    pointers->SYM_TABLE = NULL;
 
   // log("Initialization");
}
destructor static void __free__ (void) {
    BS_Free(pointers->SYM_TABLE);
    free(pointers);
    free(FILE_NAME);
    FILE_NAME = NULL;
    //log("Destruction");
}
