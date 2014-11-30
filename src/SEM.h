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



//!< DOPLNENE
typedef struct Slabel{
//!< PRVOK ZASOBNIKA LABELOV
    struct STerm * data;
    struct Slabel * previous;
}*TSlabel;

typedef struct SlabelStack{
//!< SEMANTICKY ZASOBNIK LABELOV
    struct Slabel * top;
}*TSlabelStack;
//!< DOPLNENE END



typedef struct SlistElem {
    struct STerm * constTerm;
    struct SlistElem * next;
}*TSlistElem;

typedef struct SconstList {
    struct SlistElem * first;
}*TSconstList;


// GLOBALNE PREMENNE

    extern PGLOB_DEST pointers;
    extern P3AC *EIP, *PEIP;
    extern TTerm embededFunc[];
    
// FUNKCIE PRE ZASOBNIK TYPOV
TSemStack SEM_initSS();
void SEM_pushSS(TSemStack stack, ETermType elem);
ETermType SEM_popSS(TSemStack stack);
void SEM_disposeSS(TSemStack stack);


//!< DOPLNENE
// FUNKCIE PRE ZASOBNIK LABELOV
TSlabelStack SEM_initLS();
void SEM_pushLS(TSlabelStack stack, TTerm * elem);
TTerm * SEM_popLS(TSlabelStack stack);
void SEM_disposeLS(TSlabelStack stack);
//!< DOPLNENE END


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
void SEM_defineGlobal(PTStructLex dataID, PTStructLex dataType);
/**
 * SEM_defineGlobal
 * ----------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK NASTAVA DEKLARACIA GLOBALNEJ PREMENNEJ
 * @param:dataID = LEXEMA ID PREMENNEJ
 * @param:dataType = LEXEMA TYP PREMENNEJ
 * @return: FUNKCIA VYTVORI V TABULKE SYMBOLOV UZOL PREMENNEJ
 */

 void SEM_defineParam(PTStructLex dataID, PTStructLex dataType);
 /**
 * SEM_defineParam
 * ---------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK NASTAVA DEKLARACIA PARAMETRA FUNKCIE
 * @param:dataID = LEXEMA ID PREMENNEJ
 * @param:dataType = LEXEMA TYP PREMENNEJ
 * @return: FUNKCIA VYTVORI V LOKALNEJ TABULKE SYMBOLOV UZOL PREMENNEJ
 */

void SEM_defineLocal(PTStructLex dataID, PTStructLex dataType);
/**
 * SEM_defineLocal
 * ---------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK NASTAVA DEKLARACIA LOKALNEJ PREMENNEJ
 * @param:dataID = LEXEMA ID PREMENNEJ
 * @param:dataType = LEXEMA TYP PREMENNEJ
 * @return: FUNKCIA VYTVORI V LOKALNEJ TABULKE SYMBOLOV UZOL PREMENNEJ
 */
 

void SEM_defineFunction(PTStructLex dataID);
/**
 * SEM_defineFunction
 * ------------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK SA DEFINUJE/DEKLARUJE FUNKCIA
 * @param:dataID = LEXEMA ID FUNKCIE
 * @return: VYTVORI UZOL V TABULKE SYMBOLOV A ZMENI SCOPE NA TUTO FUNKCIU
 */


void SEM_defFuntionType(PTStructLex dataType);
/**
 * SEM_defineFunction
 * ------------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK SA DEFINUJE NAVRATOVY TYP FUNKCIE
 * @param:dataType = LEXEMA TYP FUNKCIE
 * @return: VYTVORI UZOL V TABULKE SYMBOLOV A ZMENI SCOPE NA TUTO FUNKCIU
 */
 
void SEM_endFunctionDef(PTStructLex lexema);
/**
 * SEM_endFunctionDef
 * ------------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR VO CHVILI KEDY KONCI DEFINICIA/DEKLARACIA FUNKCIE (keyword forward/end funkcie)
 * @param:LEXEMA = strednik/forward
 * @return: ZMENA SCOPE NA SYM_TABLE
 */
 
//  DALSIE FUNKCIE
void SEM_assignValue(PTStructLex lexema);
/**
 * SEM_assignValue
 * ---------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR POTOM, CO SA VYHODNOTI PRAVA STRANA PRIRADENIA
 * @param:LEXEMA IDENTIFIKATOR LAVEJ STRANY
 * @return: PRIRADENIE HODNOTY VYSLEDKU
 */

 
void SEM_insertEmbFunc();
/**
 * SEM_insertEmbFunc
 * -----------------
 * @brief:FUNKCIA, KTORA NAPLNI TABULKU SYMBOLOV VSTAVANYMI FUNKCIAMI (length, copy, find, sort)
 */

 
void SEM_setFunctionLabel();
/**
 * SEM_setFunctionLabel
 * --------------------
 * @brief:FUNKCIA, KTORA VYTVORI LABEL SKOKU FUNKCIE
 */
 
void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result);
#endif