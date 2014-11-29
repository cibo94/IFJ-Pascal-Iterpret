/** Inicialization
 * \file init.h
 * \author 69DreamTeam
 * \brief All inicialization functions goes here!
 */

#ifndef h_INIT
#define h_INIT

#define constructor __attribute__((unused, constructor))
#define destructor __attribute__((unused, destructor))

typedef struct {
    TSbinstrom   SYM_TABLE;
    TSbinstrom   SCOPE;
    TTerm *      ACCREG;      //  (Accumulator)       Akumulator vysledku 
    TTerm *      SREG1;       //  (Source register 1) Operand 1
    TTerm *      SREG2;       //  (Source register 2) Operand 2
    struct SemStack   * EXPRSTACK;    //  (Expression stack)  Zasobnik vyuzivany pri typovej kontrole nad vyrazmi
    struct SconstList * CONSTLIST;    //  (Constant list)     Zoznam ukazatelov na vsetky konstanty v programe
    unsigned int       PARAMCOUNT;   //  (Parameter counter) Pocitadlo parametrov funkcie (pouzivane pri deklaracii funkcii a volani funkcii)
    
} GLOB_DEST, *PGLOB_DEST;

extern char *FILE_NAME;

FILE *init(const char *fn);
void  interpret();

#endif
