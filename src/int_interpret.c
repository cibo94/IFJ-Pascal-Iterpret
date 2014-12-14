/**
 * \file INT_parser.c
 * \author Miroslav Cibulka - xcibul10
 * \brief Interpret trojadresneho kodu
 */

#include "inc.h"
#include "init.h"

extern PGLOB_DEST pointers;
TTerm  __ebp = {
        .type = TERM_EBP
    }, __esp = {
        .type = TERM_ESP
    },*EBP = &__ebp,
      *ESP = &__esp;

__attribute__ ((unused))
P3AC *EIP, *PEIP;

TTerm *embededFunc;


/***************************************
 *      EMBEDDED function              *
 **************************************/

char *EMB_copy(char *in, int from, int size) {
    char *out = malloc(size +1);                  // Alokovanie navratoveho pola
    if (out == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!");
    memcpy(out, in + from-1, (size_t)size);                 // Kopirovanie z pozicie from do from+size do out
    out[size]=0;
    return out;
}

int EMB_length(char *s){
    char *p = s;                                            // Nastavenie ukazovatela na zaciatok pola
    while (*(p++));                                         // Posunutie ukazovatela na koniec pola
    return (int)(p-s)-1;                                    // Rozdiel medzi zaciatkom a koncom pola -1 je jeho dlzka
}


/***************************************
 *      ESP->value.esp -> funkcie      *
 **************************************/

static PTSStack SInit () {
    PTSStack ret;
    if ((ret       = malloc(sizeof(TSStack)))   == NULL ||
        (ret->term = malloc(sizeof(TTerm)*BASE_STACK_SIZE)) == NULL)
        error (ERR_INTERNAL, "Chyba alokacie zasobnika\n");
    ret->top = ret->term;
    ret->size = 0;
    return ret;
}

static bool SEmpty (PTSStack S) {
    return S->top == S->term-1;
}


static void SPush (PTSStack S, TTerm *add) {
    uint32_t size = S->size;       //!< velkost pola
    if ((size+2)%BASE_STACK_SIZE == 0) {
        if ((S->term = realloc(S->term, sizeof(TTerm)*(size+2+BASE_STACK_SIZE))) == NULL)
            error(ERR_INTERNAL, "Chyba realokacie\n");
        S->top  = S->term + size;           //!< posunutie na spravnu poziciu
                                            //!< realloc moze hodit inu adresu
        EBP->value.ebp->top  = EBP->value.ebp->size + S->term;
        EBP->value.ebp->term = S->term;
    }
    S->top++;
    memcpy(S->top, add, sizeof(TTerm));
    S->size++;
}

__attribute__((unused))
static TTerm *STop (PTSStack S) {
    return S->top;
}

static TTerm *SPop (PTSStack S) {
    TTerm *ret = S->top;
    S->top--;
    S->size--;
    return ret;
}

static TTerm *SPick (PTSStack S, int offset) {
    return S->top + offset;
}

static void SFree (PTSStack S) {
    free(S->term);
    free(S);
}

/************************************
 *        Instrukcne funkcie        *
 ***********************************/

static void plus (TTerm *op1, TTerm *op2, TTerm *ret) {
    size_t size_op1;
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            ret->value.integer = op1->value.integer + op2->value.integer;
        break;
            
        case TERM_REAL   :
            //log ("op1 %d, op2 %d\n", op1->value.real, op2->value.real);
            ret->value.real = op1->value.real + op2->value.real;
        break;

        case TERM_STRING :
            size_op1 = strlen(op1->value.string);
            ret->value.string = malloc(size_op1+strlen(op2->value.string)+1);
            if (memcpy(ret->value.string, op1->value.string, size_op1) == NULL)
                error(ERR_RNTM_OTHERS, "Chyba spajania stringov: '%s' + '%s'", op1->value.string, op2->value.string);
            if (strcat(ret->value.string, op2->value.string) == NULL)
                error(ERR_RNTM_OTHERS, "Chyba spajania stringov: '%s' + '%s'", op1->value.string, op2->value.string);
        break;

        default          : break;
    }
    ret->type = op1->type;
    ret->init = true;
}

static void minus (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            ret->value.integer = op1->value.integer - op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.real = op1->value.real - op2->value.real;
        break;

        default          :  break;
    }
    ret->type = op1->type;
    ret->init = true;
}

static void mul (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            ////log ("op1 %d, op2 %d\n", op1->value.integer, op2->value.integer);
            ret->value.integer = op1->value.integer * op2->value.integer;
        break;
        case TERM_REAL   :
            ret->value.real = op1->value.real * op2->value.real;
        break;
        default          : break;
    }
    ret->type = op1->type;
    ret->init = true;
}

static void division (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            if (op2->value.integer == 0)
                error(ERR_RNTM_ZERO, "Delenie 0 %d/%d\n", op1->value.integer, op2->value.integer);
            if (op2->type == TERM_REAL)
                ret->value.real = ((float)op1->value.integer) / op2->value.real;
            else
                ret->value.real = ((float)op1->value.integer) / ((float)op2->value.integer);
        break;
        case TERM_REAL   :
            if (op2->value.real == 0)
                 error(ERR_RNTM_ZERO, "Delenie 0 %f/%f\n", op1->value.real, op2->value.real);

            if (op2->type == TERM_REAL)
                ret->value.real = op1->value.real / op2->value.real;
            else
                ret->value.real = op1->value.real / ((float)op2->value.integer);
        break;
        default          : break;
    }
    ret->type = TERM_REAL;
    ret->init = true;
}

static void assign (TTerm *op1, 
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.integer) : op1;
    if (ret == NULL) 
        ret = STop(EBP->value.ebp);
    else
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.integer) : ret;
    ret->value = op1->value;
    ret->type = op1->type;
    ret->init = true;
}

static void less (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer < op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real < op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) < 0;
        break;

        case TERM_BOOL   :
            ret->value.boolean = op1->value.boolean < op2->value.boolean;
        break;

        default          : break;
    }
    ret->type = TERM_BOOL;
    ret->init = true;
}

static void greater (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer > op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real > op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) > 0;
        break;

        case TERM_BOOL   :
            ret->value.boolean = op1->value.boolean > op2->value.boolean;
        break;

        default          : break;
    }
    ret->type = TERM_BOOL;
    ret->init = true;
}

static void lesseq (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer <= op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real <= op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) <= 0;
        break;

        case TERM_BOOL   :
            ret->value.boolean = op1->value.boolean <= op2->value.boolean;
        break;

        default          : break;
    }
    ret->type = TERM_BOOL;
    ret->init = true;
}

static void greateq (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer >= op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real >= op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) >= 0;
        break;

        case TERM_BOOL   :
            ret->value.boolean = op1->value.boolean >= op2->value.boolean;
        break;

        default          : break;
    }
    ret->type = TERM_BOOL;
    ret->init = true;
}

static void equal (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            ret->value.boolean = op1->value.integer == op2->value.integer;
        break;
            
        case TERM_REAL   :
            ret->value.boolean = op1->value.real == op2->value.real;
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) == 0;
        break;

        case TERM_BOOL   :
            ret->value.boolean = op1->value.boolean == op2->value.boolean;
        break;

        default          : break;
    }
    ret->type = TERM_BOOL;
    ret->init = true;
}

static void nequal (TTerm *op1, TTerm *op2, TTerm *ret) {
    if (!op1->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op1->name);
    if (!op2->init) error(ERR_RNTM_UNDEF, "Praca s neinicializovanou premennou %s", op2->name);
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    if (ret->init)
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;
    switch (op1->type) {
        case TERM_INT    :
            ret->value.boolean = !(op1->value.integer == op2->value.integer);
        break;
            
        case TERM_REAL   :
            ret->value.boolean = !(op1->value.real == op2->value.real);
        break;

        case TERM_STRING :
            ret->value.boolean = strcmp(op1->value.string, op2->value.string) != 0;
        break;

        case TERM_BOOL   :
            ret->value.boolean = op1->value.boolean != op2->value.boolean;
        break;

        default          : break;
    }
    ret->type = TERM_BOOL;
    ret->init = true;
}

static void pushebp (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    TTerm EBPCPY = {
        .value.ebp = NULL
    };
    EBPCPY.value.address = EBP->value.ebp->size;
    SPush(ESP->value.esp, &EBPCPY);
    memcpy(EBP->value.ebp, ESP->value.esp, sizeof(TSStack));
}

static void popebp (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) { 
    memcpy(ESP->value.esp, EBP->value.ebp, sizeof(TSStack));
    TTerm *ptr = SPop(ESP->value.esp);
    size_t s = ptr->value.address;
    EBP->value.ebp->top  = s + ESP->value.esp->term;
    EBP->value.ebp->size = s;
    EBP->value.ebp->term = ESP->value.ebp->term;

}

static void ret (        TTerm *op1, TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    // Local variable cleaning
    for (int i = 0; i < op1->value.integer; i++) {
        SPop(ESP->value.esp);
    }
    popebp(NULL, NULL, NULL);
    
    TTerm *add = SPop(ESP->value.esp);
    P3AC *pom = &EIP[add->value.address];
    //log("RETURN: on address:%u op1:%d op2:%d\n", (uint32_t)(pom-EIP), op1->value.integer, op2->value.integer);
    PEIP = pom;
    // Arguments cleaning
    for (int i = 0; i < op2->value.integer; i++)
        SPop(ESP->value.esp);
}

static void push (       TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    SPush(ESP->value.esp, op1);
}

static void pop (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;
 
    if (ret != NULL) {
        if (!SEmpty(ESP->value.esp)) {
            TTerm *pom = SPop(ESP->value.esp);
            if (pom == NULL) error(ERR_INTERNAL, "Pop vratil NULL, ??????");
            memcpy(ret, pom, sizeof(TTerm));
        } else 
            error (ERR_INTERNAL, "Stack is empty");
    } else {
        SPop(ESP->value.esp);
    }
    ////log ("POP\n");
}

static void jmp (        TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    if (op1->value.address == 0) return;
    PEIP = &EIP[op1->value.address-1];
    ////log("JMP on adress %u with offset %u\n", (uint32_t)(PEIP-EIP), (*PEIP)->op);
}

static void jtrue (      TTerm *op1, TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
 
    if (op1->value.boolean == true) {
        jmp(op2, NULL, NULL);
    }
        ////log("Not jumping on adress %u with offset %u\n", (uint32_t)(PEIP-EIP), (*PEIP)->op);
}

static void call (       TTerm *op1, 
__attribute__ ((unused)) TTerm *op2, 
__attribute__ ((unused)) TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
 
    // Jump address
    TTerm Address = {
        .value.address = PEIP-EIP,
        .type = TERM_OFFSET,
        .index = false
    };
    push(&Address, NULL, NULL);
    for (int i = 0; i < 6; i++) {
        if (strcmp(embededFunc[i].name, op1->name) == 0) {
            pushebp(NULL, NULL, NULL);
            op1->value.emb_function();
            return;
        }
    }
    jmp(op1, NULL, NULL);


    pushebp(NULL, NULL, NULL);
    ////log ("CALL: to address: %u\n", op1->value.address);
}

static void nop (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    /// NOPE NOPE NOPE
    ////log("NOP");
    return;
}

static void not (        TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;
 
    /// Neguje vstup op1 ulozi negaciu do ret
    ret->value.boolean = !(op1->value.boolean);
}

static void load (       TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    /// nacita zo zasobnika nti prvok
    TTerm *pom = SPick(EBP->value.ebp,op1->value.integer);
    if (pom == NULL) error(ERR_INTERNAL, "Load popol NULL, %d\n\
    ESP size %u, EBP size %u\n", op1->value.offset, (unsigned)ESP->value.esp->size, 
    (unsigned)EBP->value.ebp->size);
    memcpy(ret, pom, sizeof(TTerm));
}

static void store (      TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    /// ulozi na zasobnik na presne miesto data do termu
    memcpy(SPick(EBP->value.ebp,ret->value.offset), op1, sizeof(TTerm));
}

static void __sort () {
    TTerm *str = SPick(EBP->value.ebp,-2),
          *out = SPick(EBP->value.ebp,-3),
           zero = {
               .value.integer = 0,
               .type = TERM_INT
           },
           one  = {
               .value.integer = 1,
               .type = TERM_INT
           };
    if (!str->init) 
        error(ERR_RNTM_UNDEF, "Pouzitie neinicializovanej premennej %s\n", str->name);
    out->type = TERM_STRING;
    out->init = true;
    out->value.string = EMB_sort (str->value.string, EMB_length(str->value.string));
    ret(&zero, &one, NULL);
}

static void __copy () {
    TTerm *str  = SPick(EBP->value.ebp, -4),
          *from = SPick(EBP->value.ebp, -3),
          *size = SPick(EBP->value.ebp, -2),
          *out  = SPick(EBP->value.ebp, -5),
           zero = {
               .value.integer = 0,
               .type = TERM_INT
           }, three = {
               .value.integer = 3,
               .type = TERM_INT
           };
    if (!str->init) 
        error(ERR_RNTM_UNDEF, "Pouzitie neinicializovanej premennej %s\n", str->name);
    if (!from->init) 
        error(ERR_RNTM_UNDEF, "Pouzitie neinicializovanej premennej %s\n", from->name);
    if (!size->init) 
        error(ERR_RNTM_UNDEF, "Pouzitie neinicializovanej premennej %s\n", size->name);
    out->type = TERM_STRING;
    out->init = true;
    if (from->value.integer <= 0) error(ERR_RNTM_OTHERS, "String sa indexuje od 1, zadany index %d\n", from->value.integer);
    if (strlen(str->value.string) < (unsigned)(from->value.integer + size->value.integer-1)) 
        error(ERR_RNTM_OTHERS, "Copy '%s' velkosti %u z %d do %d siaha za hranice pola", str->value.string, (unsigned)strlen(str->value.string), from->value.integer, from->value.integer + size->value.integer);
    out->value.string = EMB_copy(str->value.string, from->value.integer, size->value.integer);
   // //log ("%s", out->value.string);
    ret(&zero, &three, NULL);
}

static void __length () {
    TTerm *str = SPick(EBP->value.ebp, -2),
          *out = SPick(EBP->value.ebp, -3),
           zero= {
               .value.integer = 0
           }, 
           one = {
               .value.integer = 1
           };
    if (!str->init) 
        error(ERR_RNTM_UNDEF, "Pouzitie neinicializovanej premennej %s\n", str->name);
    out->type = TERM_INT;
    out->init = true;
    out->value.integer = EMB_length(str->value.string);
    ret(&zero, &one, NULL);
}

static void __find () {
    TTerm *str = SPick(EBP->value.ebp, -3),
          *fstr= SPick(EBP->value.ebp, -2),
          *out = SPick(EBP->value.ebp, -4),
           zero = {
               .value.integer = 0
           },
           one = {
               .value.integer = 2
           };
    if (!str->init) 
        error(ERR_INTERNAL, "Pouzitie neinicializovanej premennej %s\n", str->name);
    if (!fstr->init) 
        error(ERR_INTERNAL, "Pouzitie neinicializovanej premennej %s\n", fstr->name);
    out->type = TERM_INT;
    out->init = true;
    out->value.integer = EMB_find(str->value.string, fstr->value.string);
    ret(&zero, &one, NULL);
}

static void __write () {
    TTerm *n, k;
    int pocet = SPick(EBP->value.ebp,-2)->value.integer;
    for (int i = pocet; i > 0; i--) {
        n = SPick(EBP->value.ebp, -(2+i));
        if (!n->init) error(ERR_RNTM_UNDEF, "Pouzitie neinicializovanej premennej %s\n", n->name);
        switch (n->type) {
            case TERM_INT : 
                printf ("%d", n->value.integer);
            break;
            case TERM_REAL :
                printf ("%g", n->value.real);
            break;
            case TERM_STRING :
                printf ("%s", n->value.string);
            break;
            case TERM_BOOL :
                printf ("%s", n->value.boolean ? "TRUE" : "FALSE");
            break;

            default :
                error(ERR_INTERNAL, "dghidfghgduhdgf");
            break;
        }
    }
    k.value.integer = pocet+1;
    k.index = false;
    TTerm zero = (TTerm) { 
        .value.offset = 0,
        .type = TERM_OFFSET,
        .index = false
    };
    ret(&zero, &k, NULL);
}

static void ____readln () {
    TTerm *id = SPick(EBP->value.ebp, -2),
           one = {
               .value.integer = 1
           },
           zero = {
               .value.integer = 0
           };
    char *pom, *beg;
    int c, size;
    switch (id->value.pointer->type) {
        case TERM_INT :
            if (scanf("%d", &id->value.pointer->value.integer) != 1)
                error(ERR_RNTM_NUMREAD, "Zle nacitany integer\n");
            if (!isspace(fgetc(stdin)))
                error(ERR_RNTM_NUMREAD, "Zle nacitany integer\n");
        break;
        case TERM_REAL :
            if (scanf("%f", &id->value.pointer->value.real) != 1)
                error(ERR_RNTM_NUMREAD, "Zle nacitany real\n");
            if (!isspace(fgetc(stdin)))
                error(ERR_RNTM_NUMREAD, "Zle nacitany real\n");
        break;
        case TERM_STRING :
            beg=pom=malloc (sizeof(char)*32);
            while ((c=fgetc(stdin))!=EOF&&(c!='\n')) {
                *(pom++)=c;
                *pom='\0';
                size=pom-beg;
                if (size%32 == 0) {
                    pom=size+(beg=realloc(beg,size*2));
                    if (beg == NULL) error(ERR_INTERNAL, "Chyba realokacie!\n");
                }
            }
            id->value.pointer->value.string = beg;
        break;
        default :
            error(ERR_RNTM_NUMREAD, "Nekompatabilny typ\n");
        break;
    }
    id->value.pointer->init = true;
    ret(&zero, &one, NULL);
}


__attribute__ ((unused))
static void (*INST[])(TTerm *op1, TTerm *op2, TTerm *ret) = {
    &plus, &minus, &mul, &division, 
    &assign, &less, &greater, &lesseq, 
    &greateq, &equal, &nequal,  &call, &ret, 
    &push, &pop, &jtrue, &jmp, 
    &nop, &load, &not, &store, 
    &pushebp, &popebp
};  //!< V tomto poli su ulozene jednoduche funkcie ktore sa vyberaju podla typu operaciu
    //!< Maju presne dane parametre

void INT_interpret () {
    /// Stack pointer init
    ESP->value.esp  = SInit ();
    void * ptr = EBP->value.ebp  = malloc (sizeof(TSStack));
    for (PEIP = EIP; *PEIP != NULL; PEIP++) {
        /*
         * Hlavny cyklus, ktory splna instrukcie ktore mu dojdu zo semantiky
         */
#ifdef RUN_EIP
       printf("#%08u:\t%s\t%s%s%s%s%s\t{%f, %f, %f}\n",
          (unsigned int)(PEIP-EIP)+1,
          OPERATIONS[(*PEIP)->op],
          (*PEIP)->op1 != NULL ? (*PEIP)->op1->name            : "",
          (*PEIP)->op2 != NULL &&(*PEIP)->op1 != NULL ?  ", "  : "",
          (*PEIP)->op2 != NULL ? (*PEIP)->op2->name            : "", 
          (*PEIP)->ret != NULL &&((*PEIP)->op2 != NULL || (*PEIP)->op1 != NULL) ?  ", "   : "",
          (*PEIP)->ret != NULL ? (*PEIP)->ret->name            : "",
          (*PEIP)->op1 != NULL ? (*PEIP)->op1->value.integer   : 0, 
          (*PEIP)->op2 != NULL ? (*PEIP)->op2->value.integer   : 0,
          (*PEIP)->ret != NULL ? (*PEIP)->ret->value.integer   : 0);
#endif

        INST[(*PEIP)->op]((*PEIP)->op1, (*PEIP)->op2, (*PEIP)->ret);
    }
    SFree(ESP->value.esp);
    free(ptr);
    return;
}

void embInit() {
    embededFunc[0].value.emb_function = &__sort;
    embededFunc[0].type = TERM_EMB;
    embededFunc[0].name = "sort";
    embededFunc[0].index = false;
    embededFunc[1].value.emb_function = &__copy;
    embededFunc[1].type = TERM_EMB;
    embededFunc[1].name = "copy";
    embededFunc[1].index = false;
    embededFunc[2].value.emb_function = &__length;
    embededFunc[2].type = TERM_EMB;
    embededFunc[2].name = "length";
    embededFunc[2].index = false;
    embededFunc[3].value.emb_function = &__find;
    embededFunc[3].type = TERM_EMB;
    embededFunc[3].name = "find";
    embededFunc[3].index = false;
    embededFunc[4].value.emb_function = &__write;
    embededFunc[4].type = TERM_EMB;
    embededFunc[4].name = "write";
    embededFunc[4].index = false;
    embededFunc[5].value.emb_function = &____readln;
    embededFunc[5].type = TERM_EMB;
    embededFunc[5].name = "readln";
    embededFunc[5].index = false;
}
