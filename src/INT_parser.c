#include "inc.h"

typedef enum {
    TERM_INT,
    TERM_REAL,
    TERM_STRING,
    TERM_LABEL
} ETermType;

typedef struct {
    union {
        int       integer;
        float     real;
        char *    string;
        uint32_t  address;
    } value;
    ETermType type;
} TTerm;

static void plus (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.integer = op1->value.integer + op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.real = op1->value.real + op2->value.real;
        break;

        case TERM_STRING :
            ret->value.string = strcat(op1->value.string, op2->value.string);
        break;

        default          : break;
    }
}

static void minus (TTerm *op1, TTerm *op2, TTerm *ret) {
     switch (ret->type) {
        case TERM_INT    :
            ret->value.integer = op1->value.integer - op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.real = op1->value.real - op2->value.real;
        break;

        default          :  break;
    }
}

__attribute__((unused))
static void (*COMMANDS[])(TTerm *op1, TTerm *op2, TTerm *ret) = {
    &plus, &minus, NULL
};
