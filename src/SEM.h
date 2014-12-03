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

 
void SEM_checkFunction(PTStructLex lexema);
/**
 * SEM_checkFunction
 * -----------------
 * @brief:FUNKCIA, KTORA SA VOLA PRI PRECHODE TABULKOU SYMBOLOV. VYKONAVA DETEKCIU NEDEFINOVANYCH FUNKCII
 * @param:LEXEMA IDENTIFIKATOR PRVKU TABULKY SYMBOLOV
 * @return: ERROR AK FUNKCIA NIE JE DEFINOVANA
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
 * @brief:FUNKCIA, KTORA VYTVORI LABEL SKOKU FUNKCIE. VOLA SA NA ZACIATKU DEKLARACNEJ CASTI PRED DEFINICIOU LOKALNYCH PREMENNYCH
 */
 
 
void SEM_thenStat();
/**
 * SEM_thenStat
 * ------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY THEN V IF STATEMENTE
 * @return: VYTVORENIE POTREBNYCH LABELOV A INSTRUKCII PRE INTERPRET
 */ 
 
 
void SEM_elseStat();
/**
 * SEM_elseStat
 * ------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY ELSE V IF STATEMENTE
 * @return: VYTVORENIE SKOKU NA KONIEC IFU A NAPLNENIE LABELA ELSE
 */ 

 
void SEM_endIf();
/**
 * SEM_endIf
 * ---------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX PO UKONCENI IF STATEMENTU
 * @return: NAPLNENIE ADRESY LABELU ZA VETVOU ELSE
 */  
 
 
 void SEM_whileStat();
/**
 * SEM_whileStat
 * -------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY WHILE
 * @return: VYTVORENIE POTREBNYCH LABELOV A INSTRUKCII PRE INTERPRET
 */  
 
 
void SEM_whileBegin();
/**
 * SEM_whileBegin
 * --------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY BEGIN (PO VYHODNOTENI PODMIENKY) V CYKLE WHILE
 * @return: VYTVORENIE POTREBNYCH LABELOV A INSTRUKCII PRE INTERPRET
 */   
 

void SEM_whileEnd();
/**
 * SEM_whileBegin
 * --------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY END NA KONCI CYKLU WHILE
 * @return: UKONCENIE CYKLU WHILE
 */ 
 
void SEM_readln(PTStructLex paramID);
/**
 * SEM_whileBegin
 * --------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX NAD PARAMETROM FUNKCIE READLN
 * @return: 3AK
 */  
 
void SEM_writeCall();
/**
 * SEM_whileBegin
 * --------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX PO PREJDENI VSETKYCH PARAMETROV FUNKCIE WRITE
 * @return: 3AK
 */ 
 
 
void SEM_writePrologue(); 
/**
 * SEM_writePrologue
 * -----------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI FUNKCIE WRITE
 */ 
 
 
void SEM_prologue(); 
/**
 * SEM_prologue
 * ------------
 * @brief:PRVA VOLANA SEMANTICKA AKCIA. FUNKCIA VYGENERUJE INSTRUKCIU SKOKU NA ZACIATOK PROGRAMU
 * @return: UKONCENIE CYKLU WHILE
 */ 
 
 
void SEM_mainBegin();
/**
 * SEM_prologue
 * ------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR PO NAJDENI LEXEMY BEGIN HLAVNEHO TELA PROGRAMU
 * @return: NAPLNENIE NAVESTIA SKOKU NA ZACIATOK PROGRAMU
 */ 

 
void SEM_fCallPrologue(PTStructLex functID);
/**
 * SEM_fCallPrologue
 * -----------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR NA ZACIATKU VOLANIA FUNKCIE
 * @return: NASTAVENIE PREMENNYCH
 */ 
 
 
void SEM_functionCall(PTStructLex functID); 
/**
 * SEM_functionCall
 * ----------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR NA KONCI VOLANIA FUNKCIE (PO SPRACOVANI PARAMETROV)
 * @return: VOLANIE FUNKCIE
 */ 
 
 
void SEM_functionParam(PTStructLex functID, PTStructLex paramID);
/**
 * SEM_functionParam
 * -----------------
 * @brief:FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR NAD PARAMETRAMI FUNKCIE
 * @return: TYPOVA KONTROLA PARAMETROV, VYTVORENIE KONSTANT AK SU V PARAMETROCH
 */ 

 
void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result);

#endif
