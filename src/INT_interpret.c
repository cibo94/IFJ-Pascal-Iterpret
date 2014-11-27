/**
 * \file INT_parser.c
 * \authon Miroslav Cibulka
 * \brief Interpret trojadresneho kodu
 */

#include "inc.h"

// DUMMY typedefs:

// DUMMY functions:

///////////////////////


typedef struct SStack {
    TTerm             *term;
    struct SStack     *next;
} TStack, *PTStack;

static PTStack STACK;

static PTStack SInit () {
    return NULL;
}

static TTerm *SPosSet (PTStack *S, TTerm *address) {
    while (*S != NULL) {
        if ((*S)->next == NULL) {
            TTerm *ret = (*S)->term;
            memcpy((*S)->term, address, sizeof(TStack));
            return ret;
        }
        S++;
    }
    error(ERR_INTERNAL, "Chyba pri nastavovani instrukcneho skoku!\n");
    return NULL;
}

static TTerm *SPosGet (PTStack *S) {
    while (*S != NULL) {
        if ((*S)->next == NULL) {
            return (*S)->term;
        }
        S++;
    }
    error(ERR_INTERNAL, "Chyba pri nastavovani instrukcneho skoku!\n");
    return NULL;
}

static bool SEmpty (PTStack s) {
    return s == NULL;
}

static void SPush (PTStack *s, TTerm *add) {
    PTStack el = malloc (sizeof(TStack));
    el->next = (*s);
    el->term = add;
   *s = el;
}

static TTerm *STop (PTStack s) {
    return s->term; 
}

static TTerm *SPop (PTStack *s) {
    TTerm *add = (*s)->term;
    PTStack toDel = (*s);
    (*s) = (*s)->next;
    free(toDel);
    return add;
}

static TTerm *SPick (PTStack *s, uint32_t pos) {
    if (s == NULL) error(ERR_INTERNAL, "Stack overflow!\n");
    if (!pos) return (*s)->term;
    return SPick (&((*s)->next), pos--);
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

static void mul (TTerm *op1, TTerm *op2, TTerm *ret) {
    switch (ret->type) {
        case TERM_INT    :
            ret->value.integer = op1->value.integer * op2->value.integer;
        break;
        case TERM_REAL   :
            ret->value.real = op1->value.real * op2->value.real;
        break;
        default          : break;
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
        default          : break;
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

static void less (TTerm *op1, TTerm *op2, TTerm *ret) {
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

static void lesseq (TTerm *op1, TTerm *op2, TTerm *ret) {
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
    TTerm *Address = SPosGet(&STACK);
    SPush(&STACK, Address);
    SPosSet(&STACK, op1);
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
    TTerm *s = malloc (sizeof(TTerm));
    if (s == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    memcpy(s, op1, sizeof(TTerm));
    SPush(&STACK, s);
}

static void pop (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    if (ret == NULL) {
        if (SEmpty(STACK))
            ret = SPop(&STACK);
    } else {
        SPop(&STACK);
    }
}

static void jmp (        TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    SPosSet(&STACK, op1);
}

static void jtrue (      TTerm *op1, TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    if (op1->value.boolean == true) {
        jmp(op2, NULL, NULL);
    }
}

static void nop (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    return;
}

static void not (        TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    ret->value.boolean = !(op1->value.boolean);
}

static void load (       TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    uint32_t size = STop (STACK)->value.address;
    ret->value.pointer = SPick(&STACK, size-op1->value.address);
}

__attribute__ ((unused))
static void (*INSTRUCTIONS[])(TTerm *op1, TTerm *op2, TTerm *ret) = {
    &plus, &minus, &mul, &division, &assign, &less, &greater, &lesseq, &greateq, &equal,
    &call, &ret, &push, &pop, &jtrue, &jmp, &nop, &load, &not 
    // TODO: Pridat dalsie funkcie!
};

// TODO: vytvorit pole instrukcii:
__attribute__ ((unused))
P3AC *INST;


void INT_interpret () {
    /// Instruction pointer
    P3AC *PINST = INST;
    TTerm EIP;
    EIP.value.address = 0;
    EIP.type = TERM_EIP;
    EIP.name = NULL;

    // TODO: 
    //      * pridat volanie semantiky
    //      * pridat prechod prikazmi
    //      * pridat dealokacie
    STACK = SInit();
    SPush(&STACK, &EIP);

    // TODO: Hlavny WHILE:

    while (*PINST != NULL) {
        INSTRUCTIONS[(*PINST)->op]((*PINST)->op1, (*PINST)->op2, (*PINST)->ret);
        PINST++;
    }
    SFree (&STACK);
    return;
}
