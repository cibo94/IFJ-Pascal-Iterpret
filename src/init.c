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
    pointers->SCOPE=NULL;
    pointers->ACCREG = malloc(sizeof(struct STerm));
    if (pointers->ACCREG == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    pointers->SREG1 = malloc(sizeof(struct STerm));
    if (pointers->SREG1 == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    pointers->SREG2 = malloc(sizeof(struct STerm));
    if (pointers->SREG2 == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    pointers->EXPRSTACK = SEM_initSS();
    pointers->CONSTLIST = SEM_initCL();
    pointers->PARAMCOUNT = 0;
   // log("Initialization");
}
destructor static void __free__ (void) {
    BS_Free(pointers->SYM_TABLE);
    
    SEM_disposeCL(pointers->CONSTLIST);
    SEM_disposeSS(pointers->EXPRSTACK);
    free(pointers->ACCREG);
    free(pointers->SREG1);
    free(pointers->SREG2);
    
    free(pointers);
    free(FILE_NAME);
    FILE_NAME = NULL;
    //log("Destruction");
}
