#include "inc.h"


// DUMMY typedefs:

typedef struct S3ADD *P3ADD;

// DUMMY functions:

///////////////////////


typedef enum {
    TERM_INT,
    TERM_REAL,
    TERM_STRING,
    TERM_LABEL,
    TERM_BOOL
} ETermType;

typedef struct {
    union {
        int       integer;
        float     real;
        char     *string;
        bool      boolean;
        uint32_t  address;
    } value;
    ETermType type;
} TTerm;

typedef struct SStack {
    TTerm             *address;
    struct SStack     *next;
} TStack, *PTStack;

static PTStack STACK;

static PTStack SInit () {
    return NULL;
}

static bool SEmpty (PTStack s) {
    return s == NULL;
}

static void SPush (PTStack *s, TTerm *add) {
    PTStack el = malloc (sizeof(TStack));
    el->next = (*s);
    el->address = add;
    *s = el;
}

static TTerm *STop (PTStack s) {
    return s->address; 
}

static TTerm *SPop (PTStack *s) {
    TTerm *add = (*s)->address;
    PTStack toDel = (*s);
    (*s) = (*s)->next;
    free(toDel);
    return add;
}

static void SFree (PTStack *s) {
    if (*s == NULL) {
        return;
    }
    SFree (&((*s)->next));
    free(*s);
    *s = NULL;
}

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

static void times (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.integer = op1->value.integer * op2->value.integer;
        break;
        case TERM_REAL   :
            ret->value.real = op1->value.real * op2->value.real;
        break;
        default: break;
    }
}

static void division (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.integer = op1->value.integer / op2->value.integer;
        break;
        case TERM_REAL   :
            ret->value.real = op1->value.real / op2->value.real;
        break;
        default: break;
    }
}

static void assign (TTerm *op1, 
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.integer = op1->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.real = op1->value.real;
        break;

        case TERM_STRING :
            ret->value.string = op1->value.string;
        break;

        default          : break;
    }
}

static void smaller (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer < op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real < op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) < 0;
        break;

        default          : break;
    }
}

static void greater (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer > op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real > op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) > 0;
        break;

        default          : break;
    }
}

static void smalleq (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer <= op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real <= op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) <= 0;
        break;

        default          : break;
    }
}

static void greateq (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer >= op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real >= op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) >= 0;
        break;

        default          : break;
    }
}

static void equal (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer == op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real == op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) == 0;
        break;

        default          : break;
    }
}

static void call (       TTerm *op1, 
__attribute__ ((unused)) TTerm *op2, 
__attribute__ ((unused)) TTerm *ret) {
    SPush(&STACK, op1);
}

static void ret (        TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    for (int i = 0; i < op1->value.integer; i++)
        SPop(&STACK);
}

static void push (       TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    SPush(&STACK, op1);
}

static void pop (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    if (ret == NULL) {
        ret = SPop(STACK);
    } else {
        SPop(&STACK);
    }
}

static void jtrue (      TTerm *op1, TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    if (op1->value.boolean == true) {
        // TODO: Jump function
    }
}

static void jmp (        TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    // TODO: Jump function 
}

__attribute__ ((unused))
static void (*INSTRUCTIONS[])(TTerm *op1, TTerm *op2, TTerm *ret) = {
    &plus, &minus, &times, &division, &assign, &smaller, &greater, &smalleq, &greateq, &equal,
    &call, &ret, &push, &pop, &jtrue, &jmp
};

__attribute__ ((unused))
static P3ADD *SEM_CMD;

__attribute__ ((unused))
void INT_parse () {
    // TODO: 
    //      * pridat volanie semantiky
    //      * pridat prechod prikazmi
    //      * pridat dealokacie
    STACK = SInit();
    SFree (&STACK);
    return;
}
