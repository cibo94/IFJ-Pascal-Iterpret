/**
 * \file SEM.h
 * \brief Kniznica definujuca struktury a funkcie potrebne pre semanticku analyzu
 * \author 69DreamTeam
 */

#ifndef h_SEM
#define h_SEM

// DATOVE TYPY

typedef struct StackElem{
//!< PRVOK SEMANTICKEHO ZASOBNIKA
    ETermType data;
    struct StackElem *previous;
}*TStackElem;


typedef struct SemStack{
//!< SEMANTICKY VYRAZOVY ZASOBNIK
    struct StackElem *top; 
}*TSemStack;


typedef struct SlistElem {
    struct STerm * constTerm;
    struct SlistElem * next;
}*TSlistElem;

typedef struct SconstList {
    struct SlistElem * first;
}*TSconstList;


// GLOBALNE PREMENNE

    extern PGLOB_DEST pointers;
    
// FUNKCIE PRE ZASOBNIK

TSemStack SEM_initSS();
void SEM_pushSS(TSemStack stack, ETermType elem);
ETermType SEM_popSS(TSemStack stack);
void SEM_disposeSS(TSemStack stack);

// FUNKCIE PRE ZOZNAM KONSTANT
TSconstList SEM_initCL();
void SEM_addCL(TSconstList list, TTerm * elem);
void SEM_disposeCL(TSconstList list);


// FUNKCIE PRE ABSTRAKTNY SYNTAKTICKY STROM

void SEM_createLeaf(PTStructLex lexema);
/**
 * SEM_createLeaf
 * --------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK SA VO VYRAZE VYSKYTNE IDENTIFIKATOR PREMENNEJ ALEBO KONSTANTA
 * @param:LEXEMA = LEXEMA PREMENNEJ TYPU ID ALEBO KONSTANTA
 * @return: FUNKCIA SKONTROLUJE CI JE PREMENNA POUZITELNA VO VYRAZE. AK SA JEDNA O KONSTANTU TAK JU VYTVORI A PATRICNE ULOZI. FUNKCIA GENERUJE 3AK PRE INTERPRET
 */
 
 
void SEM_createTree(PTStructLex lexema);
/**
 * SEM_createLeaf
 * --------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK SA VO VYRAZE KONA REDUKCIA 2 PRVKOV NA JEDEN
 * @param:LEXEMA = LEXEMA PREMENNEJ TYPU OPERATOR (ARITMETICKE AJ LOGICKE)
 * @return: FUNKCIA VYKONA TYPOVU KONTROLU OBOCH PRVKOV A VYGENERUJE 3AK INSTRUKCIU
 */
 
 
//  FUNKCIE POTREBNE PRI DEFINICII A DEKLARACII PREMENNYCH A FUNKCII
void SEM_varDec(PTStructLex lexema);
/**
 * SEM_varDec
 * ----------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK NASTAVA DEKLARACIA PREMENNEJ (BEZ OHLADU NA UROVEN)
 * @param:LEXEMA = LEXEMA PREMENNEJ TYPU ID
 * @return: FUNKCIA VYTVORI V TABULKE SYMBOLOV UZOL PREMENNEJ
 */
 

void SEM_funcDef(PTStructLex lexema);
/**
 * SEM_funcDef
 * -----------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK SA DEFINUJE/DEKLARUJE FUNKCIA
 * @param:LEXEMA = LEXEMA ID FUNKCIE
 * @return: VYTVORI UZOL V TABULKE SYMBOLOV A ZMENI SCOPE NA TUTO FUNKCIU
 */

 
void SEM_typeDefinition(PTStructLex lexema, PTStructLex term_lex);
/**
 * SEM_typeDefinition
 * ------------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR POKIAL PRIRADUJE PREMENNEJ/FUNKCII TYP
 * @param:LEXEMA = LEXEMA TYPU (integer, real...)
 * @param:TERM_LEX = LEXEMA PREMENNEJ/FUNKCIE TYPU ID
 * @return: NASTAVI SPRAVNY TYP TERMU V UZLE, PRE LOKALNE PREMENNE NASTAVUJE STRING PARAMETROV
 */
 
 
void SEM_funcEnd();
/**
 * SEM_funcEnd
 * -----------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR VO CHVILI KEDY KONCI DEFINICIA/DEKLARACIA FUNKCIE (keyword forward/end funkcie)
 * @return: ZMENA SCOPE NA SYM_TABLE
 */
 
//  DALSIE FUNKCIE
void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result);
#endif