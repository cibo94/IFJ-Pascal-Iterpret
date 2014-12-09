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
    pointers->CURRENTFUNCT =NULL;
    pointers->ACCREG = malloc(sizeof(struct STerm));
    if (pointers->ACCREG == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    pointers->ACCREG->value.integer = 1;
    pointers->ACCREG->init  = false;
    pointers->ACCREG->index = false;
    pointers->ACCREG->name  = "AccREG";
    pointers->SREG1 = malloc(sizeof(struct STerm));
    if (pointers->SREG1 == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    pointers->SREG1->value.integer = 2;
    pointers->SREG1->init  = false;
    pointers->SREG1->index = false;
    pointers->SREG1->name  = "SREG1";
    pointers->SREG2 = malloc(sizeof(struct STerm));
    if (pointers->SREG2 == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    pointers->SREG2->value.integer = 3;
    pointers->SREG2->init  = false;
    pointers->SREG2->index = false;
    pointers->SREG2->name  = "SREG2";
    pointers->EXPRSTACK = SEM_initSS();
    pointers->LABELSTACK = SEM_initLS();
    pointers->CONSTLIST = SEM_initCL();
    pointers->PARAMCOUNT = 0;
    pointers->PROGRAMINDEX = 0;
    if ((PEIP = EIP = malloc (sizeof(P3AC)*EIP_SIZE)) == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    *PEIP = NULL;
    embededFunc = malloc(sizeof(TTerm)*6);
    embInit();
   // log("Initialization");
}
destructor static void __free__ (void) {
    BS_Free(pointers->SYM_TABLE);
    
    SEM_disposeCL(pointers->CONSTLIST);
    SEM_disposeSS(pointers->EXPRSTACK);
    SEM_disposeLS(pointers->LABELSTACK);
    free(pointers->ACCREG);
    free(pointers->SREG1);
    free(pointers->SREG2);
    for (PEIP = EIP; *PEIP != NULL; PEIP++)
        free(*PEIP);
    free(EIP);
    free(pointers);
    free(FILE_NAME);
    free(embededFunc);
    FILE_NAME = NULL;
    //log("Destruction");
}
