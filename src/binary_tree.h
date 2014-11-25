#ifndef h_SYM
#define h_SYM

typedef struct Sbinstrom {
    PTStructLex data;               //<! lexema
    uint32_t key;                   //<! Kluc
    struct Sbinstrom *loc_table;    //<! podstrom s lokalnou tabulkou symbolov 
    struct Sbinstrom *rptr;         //<! Lavy branch
    struct Sbinstrom *lptr;         //<! Pravy branch
} *TSbinstrom;                      //<! Strom


#endif

/**
 * alokuje koren binarneho stromu a napni ho datami,
 * vrati pointer nan, v pripade chyby vrati NULL
 */
TSbinstrom BS_New(PTStructLex data);

/**
 * ak najde kluc, tak aktualizuje data, inak
 * vytvori a prida novy uzol do binarneho stromu
 * a naplni ho datami a zaradi podla kluca
 */
TSbinstrom BS_Add(TSbinstrom root, PTStructLex data);

/**
 * Hlada uzol podla kluca a vrati ukazovatel
 * na najdeny uzol alebo NULL
 */
TSbinstrom BS_Find(TSbinstrom root, PTStructLex dat);

/**
 * pomocou PostOrder prejde stromom a uvolni vsetky uzly 
 */
void BS_Free(TSbinstrom root);
 
/**
 * s PostOrder prejde stromom a vypise kluce vsetkych uzlov 
 */
void BS_Print(TSbinstrom root);
