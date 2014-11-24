#include "inc.h"

typedef enum {
    TERM_INT,
    TERM_REAL,
    TERM_STRING
} ETermType;

typedef struct {
    int    ival;
    float  dval;
    char  *sval;
    ETermType type;
} TTerm;

static void plus (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->ival = op1->ival + op2->ival;
        break;
            
        case TERM_REAL   :
            ret->dval = op1->dval + op2->dval;
        break;

        case TERM_STRING :
            ret->sval = strcat(op1->sval, op2->sval);
        break;
    }
}

static void minus (TTerm *op1, TTerm *op2, TTerm *ret) {
     switch (ret->type) {
        case TERM_INT    :
            ret->ival = op1->ival - op2->ival;
        break;
            
        case TERM_REAL   :
            ret->dval = op1->dval - op2->dval;
        break;

        case TERM_STRING :
        break;
    }
}

__attribute__((unused))
static void (*COMMANDS[])(TTerm *op1, TTerm *op2, TTerm *ret) = {
    &plus, &minus, NULL
};
