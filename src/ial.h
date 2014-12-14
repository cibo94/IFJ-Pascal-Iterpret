/**
 * \file ial.h
 * \author Roman Selecky - xselec01, Miroslav Cibulka - xcibul10, Lukas Slouka - xslouk02
 * \brief Tabulka symbolov
 *
 *
 * MODELOVE VOLANIA
 * ========================================================
 * pre pridanie uzlu (aj uplne prveho) do tabulky symbolov 
 * alebo tabulky lokalnych premennych volame:
 *  
 * 		pom=BS_Add(NULL,lex);  	pre prvy uzol
 * 		BS_Add(pom,lex2);		pre dalsie uzly
 * 
 * pre pridavanie do lokalnych tabuliek treba napnit 
 * vysledok prveho pridavania do pom->loc_table;
 *
 *
 * Pre hladanie v tabulke symbolov alebo jednotlivych 
 * scope-och pouzivame funkciu:
 *
 * 		BS_Find(pom,lex2)		ak existuje vrati ukazatel
 * 
 *
 *
 * Cela Tabulka symbolov a aj lokalne tabulky sa uvolnia:
 * 
 * 		BS_Free(pom);
 */

#ifndef h_SYM
#define h_SYM

typedef struct Sbinstrom {
    PTStructLex data;               //<! lexema
    uint32_t key;                   //<! Kluc
    struct Sbinstrom *loc_table;    //<! podstrom s lokalnou tabulkou symbolov 
    struct Sbinstrom *rptr;         //<! Lavy branch
    struct Sbinstrom *lptr;         //<! Pravy branch
} *TSbinstrom;                      //<! Strom


/**
 * \brief alokuje koren binarneho stromu a napni ho datami,
 *        vrati pointer nan, v pripade chyby vrati NULL
 * \param data Data ktore sa pichnu na zaciatok stromu
 * \return Binary Tree
 */
TSbinstrom BS_New(PTStructLex data);

/**
 * \brief ak najde kluc,ERR, inak
 *        vytvori a prida novy uzol do binarneho stromu
 *        a naplni ho datami a zaradi podla kluca
 * \param root Koren stromu
 * \param data Data na vlozenie
 * \return Pridany prvok
 */
TSbinstrom BS_Add(TSbinstrom root, PTStructLex data);

/**
 * \brief Hlada uzol podla kluca a vrati ukazovatel
 *        na najdeny uzol alebo NULL
 * \param root koren
 * \param dat Data podla, ktorych ma hladat prvok
 * \return Hladany prvok
 */
TSbinstrom BS_Find(TSbinstrom root, PTStructLex dat);

/**
 * \brief pomocou PostOrder prejde stromom a uvolni vsetky uzly 
 * \param root Koren stromu
 */
void BS_Free(TSbinstrom root);
 

/**
 * \brief 
 * \param root Koren
 */
void BS_checkFunction(TSbinstrom root);

/** Sort function
 * \brief Embeded function for sorting
 * \param in Input array
 * \param size size of array
 * \return Output array
 */
char *EMB_sort(char *in, int size);

/** Find function
 * \brief Embeded function for finding substrings (KMP algorithm)
 * \param *s Input string
 * \param *search Substring to be found
 * \return Index of first occurence of substring (Pascal notation), 0 if no substring to be found
*/
int EMB_find(char *s, char *search);

#endif
