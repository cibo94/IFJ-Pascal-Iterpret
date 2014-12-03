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
    TSbinstrom   SYM_TABLE;   //!<  Ukazovatel na tabulku symbolov
    TSbinstrom   SCOPE;       //!<  Ukazovatel na uzol urcujuci Scope
    TSbinstrom   CURRENTFUNCT;//!<  Ukazovatel na uzol urcujuci danu funkciu
    TTerm *      ACCREG;      //!<  (Accumulator)       Akumulator vysledku 
    TTerm *      SREG1;       //!<  (Source register 1) Operand 1
    TTerm *      SREG2;       //!<  (Source register 2) Operand 2
    struct SemStack   * EXPRSTACK;    //!<  (Expression stack)  Zasobnik vyuzivany pri typovej kontrole nad vyrazmi
    struct SconstList * CONSTLIST;    //!<  (Constant list)     Zoznam ukazatelov na vsetky konstanty v programe
    struct SlabelStack* LABELSTACK;   //!<  (Label stack)       Zasobnik vyuzivany pri skokoch a navestiach
    unsigned int       PARAMCOUNT;    //!<  (Parameter counter) Pocitadlo parametrov funkcie (pouzivane pri deklaracii funkcii a volani funkcii)
    unsigned int       PROGRAMINDEX;
    PTSStack           ebp;
    
} GLOB_DEST, *PGLOB_DEST;

extern char *FILE_NAME;         //!< meno zdrojaku

FILE *init(const char *fn);     //!< inicializacna funckia

#endif
