/**
 * \file sem.h
 * \brief Kniznica definujuca struktury a funkcie potrebne pre semanticku analyzu
 * \author Lukas Slouka - xslouk02
 */

#ifndef h_SEM
#define h_SEM

typedef struct StackElem{
    ETermType data;             //!< data
    struct StackElem *previous; //!< predchadzajuci prvok
}*TStackElem;                   //!< Prvok VYRAZOVeho zasobnika

typedef struct SemStack{
    struct StackElem *top;      //!< Vrchol zasobnika
}*TSemStack;                    //!< SEMANTICKY VYRAZOVY ZASOBNIK

typedef struct Slabel{
    struct STerm * data;        //!< data
    struct Slabel * previous;   //!< predchadzajuci prvok
}*TSlabel;                      //!< prvok zasobnika labelov

typedef struct SlabelStack{
    struct Slabel * top;        //!< vrchol zasobnika labelov
}*TSlabelStack;                 //!< SEMANTICKY ZASOBNIK LABELOV

typedef struct SlistElem {
    struct STerm * constTerm;   //!< Data
    struct SlistElem * next;    //!< Next pointer
}*TSlistElem;                   //!< prvok Constant Listu

typedef struct SconstList {
    struct SlistElem * first;   //!< Prvy prvok zoznamu
}*TSconstList;                  //!< Konstant list

extern PGLOB_DEST pointers;     //!< pointrovy kos
extern P3AC *EIP, *PEIP;        //!< pole instrukcii
extern TTerm *embededFunc;      //!< pole vstavanych funkcii

/**
 * SEM_initSS
 * ----------
 * \brief Inicializacia zasobnika typov
 * \return Zasobnik typov
 */    
TSemStack SEM_initSS();

/**
 * SEM_pushSS
 * ----------
 * \brief Push elementu na stack typov 
 * \param stack zasobnik 
 * \param elem prvok na pushnutie
 */
void SEM_pushSS(TSemStack stack, ETermType elem);

/**
 * SEM_popSS
 * ---------
 * \brief Pop zo zasobnika TYPOV 
 * \param stack Zasobnik 
 * \return Popnuty element
 */
ETermType SEM_popSS(TSemStack stack);

/**
 * SEM_disposeSS
 * -------------
 * \brief Dispose zasobnika TYPOV
 * \param stack zasobnik
 */
void SEM_disposeSS(TSemStack stack);

/**
 * SEM_initLS
 * ----------
 * \brief Inicializacia label zasobnika
 * \return zasobnik
 */
TSlabelStack SEM_initLS();

/**
 * SEM_pushLS
 * ----------
 * \brief Push elementu do ZASOBNIKA
 * \param stack ZASOBNIK 
 * \param elem element
 */
void SEM_pushLS(TSlabelStack stack, TTerm * elem);

/**
 * SEM_popLS
 * ---------
 * \brief Pop prvku zo ZASOBNIKA
 * \param stack zasobnik
 * \return prvok TTerm
 */
TTerm * SEM_popLS(TSlabelStack stack);

/**
 * SEM_disposeLS
 * -------------
 * \brief Dispose label stacku
 * \param stack zasobnik
 */
void SEM_disposeLS(TSlabelStack stack);

/**
 * SEM_initCL
 * ----------
 * \brief nainicializuje zoznam 
 * \return Zoznam
 */
TSconstList SEM_initCL();

/**
 * SEM_addCL
 * ---------
 * \brief Prida prvok do zoznamu kosnstant
 * \param list Zoznam
 * \param elem prvok na pridanie do zoznamu
 */
void SEM_addCL(TSconstList list, TTerm * elem);

/**
 * SEM_disposeCL
 * -------------
 * \brief Dispozne constant list
 * \param list Zoznam
 */
void SEM_disposeCL(TSconstList list);

/**
 * SEM_createLeaf
 * --------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR 
 *        AK SA VO VYRAZE VYSKYTNE IDENTIFIKATOR PREMENNEJ ALEBO KONSTANTA
 * \param  LEXEMA LEXEMA PREMENNEJ TYPU ID ALEBO KONSTANTA
 * \return FUNKCIA SKONTROLUJE CI JE PREMENNA POUZITELNA VO VYRAZE. 
 *         AK SA JEDNA O KONSTANTU TAK JU VYTVORI A PATRICNE ULOZI. FUNKCIA GENERUJE 3AK PRE INTERPRET
 */
void SEM_createLeaf(PTStructLex lexema);

 /**
 * SEM_createLeaf
 * --------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK SA VO VYRAZE KONA REDUKCIA 2 PRVKOV NA JEDEN
 * \param LEXEMA LEXEMA PREMENNEJ TYPU OPERATOR (ARITMETICKE AJ LOGICKE)
 * \return  FUNKCIA VYKONA TYPOVU KONTROLU OBOCH PRVKOV A VYGENERUJE 3AK INSTRUKCIU
 */
void SEM_createTree(PTStructLex lexema);
 
/**
 * SEM_defineGlobal
 * ----------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK NASTAVA DEKLARACIA GLOBALNEJ PREMENNEJ
 * \param dataID LEXEMA ID PREMENNEJ
 * \param dataType LEXEMA TYP PREMENNEJ
 * \return  FUNKCIA VYTVORI V TABULKE SYMBOLOV UZOL PREMENNEJ
 */
void SEM_defineGlobal(PTStructLex dataID, PTStructLex dataType);

/**
 * SEM_defineParam
 * ---------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK NASTAVA DEKLARACIA PARAMETRA FUNKCIE
 * \param dataID = LEXEMA ID PREMENNEJ
 * \param dataType = LEXEMA TYP PREMENNEJ
 * \return  FUNKCIA VYTVORI V LOKALNEJ TABULKE SYMBOLOV UZOL PREMENNEJ
 */
 void SEM_defineParam(PTStructLex dataID, PTStructLex dataType);
 
/**
 * SEM_defineLocal
 * ---------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK NASTAVA DEKLARACIA LOKALNEJ PREMENNEJ
 * \param dataID = LEXEMA ID PREMENNEJ
 * \param dataType = LEXEMA TYP PREMENNEJ
 * \return  FUNKCIA VYTVORI V LOKALNEJ TABULKE SYMBOLOV UZOL PREMENNEJ
 */
void SEM_defineLocal(PTStructLex dataID, PTStructLex dataType);

/**
 * SEM_defineFunction
 * ------------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK SA DEFINUJE/DEKLARUJE FUNKCIA
 * \param dataID = LEXEMA ID FUNKCIE
 * \return  VYTVORI UZOL V TABULKE SYMBOLOV A ZMENI SCOPE NA TUTO FUNKCIU
 */
void SEM_defineFunction(PTStructLex dataID);

/**
 * SEM_defineFunction
 * ------------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR AK SA DEFINUJE NAVRATOVY TYP FUNKCIE
 * \param dataType = LEXEMA TYP FUNKCIE
 * \return  VYTVORI UZOL V TABULKE SYMBOLOV A ZMENI SCOPE NA TUTO FUNKCIU
 */
void SEM_defFuntionType(PTStructLex dataType);

/**
 * SEM_endFunctionDef
 * ------------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR VO CHVILI 
 *        KEDY KONCI DEFINICIA/DEKLARACIA FUNKCIE (keyword forward/end funkcie)
 * \param LEXEMA = strednik/forward
 * \return  ZMENA SCOPE NA SYM_TABLE
 */
void SEM_endFunctionDef(PTStructLex lexema);

/**
 * SEM_assignValue
 * ---------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR POTOM, CO SA VYHODNOTI PRAVA STRANA PRIRADENIA
 * \param LEXEMA IDENTIFIKATOR LAVEJ STRANY
 * \return  PRIRADENIE HODNOTY VYSLEDKU
 */
void SEM_assignValue(PTStructLex lexema);

/**
 * SEM_checkFunction
 * -----------------
 * \brief FUNKCIA, KTORA SA VOLA PRI PRECHODE TABULKOU SYMBOLOV. VYKONAVA DETEKCIU NEDEFINOVANYCH FUNKCII
 * \param LEXEMA IDENTIFIKATOR PRVKU TABULKY SYMBOLOV
 * \return  ERROR AK FUNKCIA NIE JE DEFINOVANA
 */
void SEM_checkFunction(PTStructLex lexema); 
 
/**
 * SEM_insertEmbFunc
 * -----------------
 * \brief FUNKCIA, KTORA NAPLNI TABULKU SYMBOLOV VSTAVANYMI FUNKCIAMI (length, copy, find, sort)
 */
void SEM_insertEmbFunc();

/**
 * SEM_setFunctionLabel
 * --------------------
 * \brief FUNKCIA, KTORA VYTVORI LABEL SKOKU FUNKCIE. 
 *        VOLA SA NA ZACIATKU DEKLARACNEJ CASTI PRED DEFINICIOU LOKALNYCH PREMENNYCH
 */
void SEM_setFunctionLabel();
 
/**
 * SEM_thenStat
 * ------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY THEN V IF STATEMENTE
 * \return  VYTVORENIE POTREBNYCH LABELOV A INSTRUKCII PRE INTERPRET
 */
void SEM_thenStat();
 
/**
 * SEM_noElse
 * ----------
 * \brief FUNKCIA, KTORU VOLA SYNTAX AK SA UKONCI PRIKAZ IF-THEN
 * \return  NAPLNENIE LABELOV TAK AKO JE TO TREBA
 */
void SEM_noElse();
  
/**
 * SEM_elseStat
 * ------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY ELSE V IF STATEMENTE
 * \return  VYTVORENIE SKOKU NA KONIEC IFU A NAPLNENIE LABELA ELSE
 */
void SEM_elseStat();

/**
 * SEM_endIf
 * ---------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO UKONCENI IF STATEMENTU
 * \return  NAPLNENIE ADRESY LABELU ZA VETVOU ELSE
 */
void SEM_endIf();
  
/**
 * SEM_whileStat
 * -------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY WHILE
 * \return  VYTVORENIE POTREBNYCH LABELOV A INSTRUKCII PRE INTERPRET
 */
void SEM_whileStat();
 
/**
 * SEM_whileBegin
 * --------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY BEGIN (PO VYHODNOTENI PODMIENKY) V CYKLE WHILE
 * \return  VYTVORENIE POTREBNYCH LABELOV A INSTRUKCII PRE INTERPRET
 */ 
void SEM_whileBegin();
 
/**
 * SEM_whileBegin
 * --------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY END NA KONCI CYKLU WHILE
 * \return  UKONCENIE CYKLU WHILE
 */ 
void SEM_whileEnd();
 
/**
 * SEM_repeatStat
 * --------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI LEXEMY REPEAT
 * \return  VYTVORENIE A NAPLNENIE LABELU SKOKU NA REPEAT
 */
void SEM_repeatStat();
 
/**
 * SEM_repeatEnd
 * -------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO VYHODNOTENI PODMIENKY ZA LEXEMEOU UNTIL
 * \return  SKOK NA LABEL REPEAT AK NIE JE PODMIENKA SPLNENA
 */ 
void SEM_repeatEnd();

  
/**
 * SEM_whileBegin
 * --------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX NAD PARAMETROM FUNKCIE READLN
 * \return  3AK
 */ 
void SEM_readln(PTStructLex paramID);
 
/**
 * SEM_whileBegin
 * --------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO PREJDENI VSETKYCH PARAMETROV FUNKCIE WRITE
 * \return  3AK
 */
void SEM_writeCall();
 
/**
 * SEM_writePrologue
 * -----------------
 * \brief FUNKCIA, KTORU VOLA SYNTAX PO NAJDENI FUNKCIE WRITE
 */
void SEM_writePrologue(); 
 
/**
 * SEM_prologue
 * ------------
 * \brief PRVA VOLANA SEMANTICKA AKCIA. FUNKCIA VYGENERUJE INSTRUKCIU SKOKU NA ZACIATOK PROGRAMU
 * \return  UKONCENIE CYKLU WHILE
 */
void SEM_prologue(); 
 
/**
 * SEM_prologue
 * ------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR PO NAJDENI LEXEMY BEGIN HLAVNEHO TELA PROGRAMU
 * \return  NAPLNENIE NAVESTIA SKOKU NA ZACIATOK PROGRAMU
 */
void SEM_mainBegin();

/**
 * SEM_fCallPrologue
 * -----------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR NA ZACIATKU VOLANIA FUNKCIE
 * \return  NASTAVENIE PREMENNYCH
 */
void SEM_fCallPrologue(PTStructLex functID);
 
/**
 * SEM_functionCall
 * ----------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR NA KONCI VOLANIA FUNKCIE (PO SPRACOVANI PARAMETROV)
 * \return VOLANIE FUNKCIE
 */
void SEM_functionCall(PTStructLex functID); 
 
/**
 * SEM_functionParam
 * -----------------
 * \brief FUNKCIA, KTORU VOLA SYNTAKTICKY ANALYZATOR NAD PARAMETRAMI FUNKCIE
 * \return TYPOVA KONTROLA PARAMETROV, VYTVORENIE KONSTANT AK SU V PARAMETROCH
 */ 
void SEM_functionParam(PTStructLex functID, PTStructLex paramID);

/**
 * SEM_generate
 * ------------
 * \brief Funkcia generuje pole instrukcii
 * \param operation Operacia
 * \param op1 Operand 1
 * \param op2 Operand 2
 * \param result Vysledok operacie
 */
void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result);

#endif
