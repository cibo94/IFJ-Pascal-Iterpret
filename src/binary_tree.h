#ifndef h_SYM
#define h_SYM

typedef struct Sbinstrom {
    void *data;                     //<! Dynamicke pretypovanie
    uint64_t key;                   //<! Kluc
    struct Sbinstrom *rptr;         //<! Lavy branch
    struct Sbinstrom *lptr;         //<! Pravy branch
} *TSbinstrom;                      //<! Strom


#endif
TSbinstrom BS_New(void *data,uint64_t key);
/**
 * alokuje koren binarneho stromu a napni ho datami,
 * vrati pointer nan, v pripade chyby vrati NULL
 */

void BS_Add(TSbinstrom root, void *data, uint64_t key);
/**
 * ak najde kluc, tak aktualizuje data, inak
 * vytvori a prida novy uzol do binarneho stromu
 * a naplni ho datami a zaradi podla kluca
 */

TSbinstrom BS_Find(TSbinstrom root, uint64_t key);
/**
 * Hlada uzol podla kluca a vrati ukazovatel
 * na najdeny uzol alebo NULL
 */

 void BS_Free(TSbinstrom root);
 /**
 * pomocou PostOrder prejde stromom a uvolni vsetky uzly 
 */

 void BS_Print(TSbinstrom root);
/**
 * s PostOrder prejde stromom a vypise kluce vsetkych uzlov 
 */

void BS_Hash (PTStructLex lex);
