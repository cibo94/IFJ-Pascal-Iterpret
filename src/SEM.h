/**
 * \file SEM.h
 * \brief Kniznica definujuca struktury a funkcie potrebne pre semanticku analyzu
 * \author 69DreamTeam
 */

#ifndef h_SEM
#define h_SEM

// DATOVE TYPY
typedef enum {
    TERM_INT,
    TERM_REAL,
    TERM_STRING,
    TERM_LABEL,
    TERM_BOOL,
    TERM_EIP,
    TERM_POINTER
} ETermType;

typedef enum {
    OP_PLUS, 
    OP_MINUS, 
    OP_MUL, 
    OP_DIV, 
    OP_ASSING, 
    OP_LESS,
    OP_GREAT,
    OP_LESSEQ,
    OP_GREATEQ, 
    OP_EQUAL,
    OP_CALL, 
    OP_RET, 
    OP_PUSH, 
    OP_POP, 
    OP_JTRUE, 
    OP_JMP, 
    OP_NOP, 
    OP_LOAD,
    OP_NOT 
} E_OP;

typedef struct StackElem{
//!< PRVOK SEMANTICKEHO ZASOBNIKA
    struct ASS *data;
    struct StackElem *previous;
}*TStackElem;


typedef struct SemStack{
//!< SEMANTICKY VYRAZOVY ZASOBNIK
    struct StackElem *top; 
}*TSemStack;


typedef struct ASS{
//!< ABSTRAKTNY SYNTAKTICKY STROM
    TTerm *data;
    uint32_t flags;
    struct ASS *rptr;
    struct ASS *lptr;
}*TASS;

typedef struct S3AC {
    E_OP   op;
    TTerm *op1;
    TTerm *op2;
    TTerm *ret;
}*P3AC;

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
TASS SEM_popSS(TSemStack stack);
void SEM_disposeSS(TSemStack stack);

// FUNKCIE PRE ABSTRAKTNY SYNTAKTICKY STROM

void SEM_createLeaf(TSemStack stack, ESyntaxRule pravidlo);
void SEM_createTree(TSemStack stack, ESyntaxRule pravidlo, ERelationOperator wParam);
void SEM_functionCall();
void SEM_ASSdispose(TASS tree);

//  DALSIE FUNKCIE SEMANTICKEJ ANALYZY

void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result);
#endif