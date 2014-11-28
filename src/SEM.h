/**
 * \file SEM.h
 * \brief Kniznica definujuca struktury a funkcie potrebne pre semanticku analyzu
 * \author 69DreamTeam
 */
/*
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


typedef struct SconstList {
    TSlistElem first;
}*TSconstList;

typedef struct SlistElem {
    TTerm * constTerm;
    TSlistElem next;
}*TSlistElem;


// GLOBALNE PREMENNE

    extern TSemStack    GLOBAL_SEM_ExpStack;
    extern int          GLOBAL_SEM_localVarCount;
    extern PTStructLex  lexema;
    extern TSbinstrom   GLOBAL_scope;
    extern TSbinstrom   GLOBAL_sym_table;
    extern TTerm *      GLOBAL_SEM_funcEAX;
    extern TProgram     GLOBAL_SEM_Program;
    
// FUNKCIE PRE ZASOBNIK

void SEM_initSS(TSemStack stack);
void SEM_pushSS(TSemStack stack, TStackElem uzol);
ETermType SEM_popSS(TSemStack stack);
void SEM_disposeSS(TSemStack stack);

// FUNKCIE PRE ZOZNAM KONSTANT

void SEM_initCL(TSconstList list);
void SEM_addCL(TSconstList list, TTerm * elem);
void SEM_disposeCL(TSconstList list);


// FUNKCIE PRE ABSTRAKTNY SYNTAKTICKY STROM

void SEM_createLeaf(TSemStack stack, ESyntaxRule pravidlo);
void SEM_createTree(TSemStack stack, ESyntaxRule pravidlo, ERelationOperator wParam);
void SEM_functionCall();
void SEM_ASSdispose(TASS tree);

//  DALSIE FUNKCIE SEMANTICKEJ ANALYZY

void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result);
#endif*/