/**
 * \file INT_parser.c
 * \author Miroslav Cibulka
 * \brief Interpret trojadresneho kodu
 */

#include "inc.h"
#include "init.h"

extern PGLOB_DEST pointers;
TTerm  __ebp = {
        .type = TERM_EBP
    }, __esp = {
        .type = TERM_ESP
    }, __bebp = {
        .type = TERM_EBP
    },
      *BEBP= &__bebp,
      *EBP = &__ebp,
      *ESP = &__esp;

__attribute__ ((unused))
P3AC *EIP, *PEIP;


/***************************************
 *      EMBEDDED function              *
 **************************************/

char *EMB_copy(char *in, int from, int size) {
    char *out = malloc((size-from+1)*sizeof(char) +1);                  // Alokovanie navratoveho pola
    if (out == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!");
    memcpy(out, in + from-1, (size_t)size-from+1);                 // Kopirovanie z pozicie from do from+size do out
    out[size-from+1]=0;
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
        (ret->term = malloc(sizeof(TTerm*)*32)) == NULL)
        error (ERR_INTERNAL, "Chyba alokacie zasobnika\n");
    ret->top = ret->term;
    *ret->top = NULL;
    ret->size = 0;
    return ret;
}

static bool SEmpty (PTSStack S) {
    return S->top == NULL;
}
/*
static void *s_realloc (TTerm ** arr, size_t s) {
    TTerm ** swap = malloc (sizeof(TTerm*)*(s+2+32));
    memcpy(swap, arr, s+2);
    free(arr);
}*/

static void SPush (PTSStack S, TTerm *add) {
    uint32_t size = S->size;       //!< velkost pola
    if ((size+2)%32 == 0) {
        if ((S->term = realloc(S->term, sizeof(TTerm*)*(size+2+32))) == NULL)
            error(ERR_INTERNAL, "Chyba realokacie\n");
        S->top  = S->term + size;           //!< posunutie na spravnu poziciu
                                            //!< realloc moze hodit inu adresu
        EBP->value.ebp->top  = EBP->value.ebp->size + S->term;
        EBP->value.ebp->term = S->term;
    }
    S->top++;
    *S->top = malloc (sizeof(TTerm));
    if (*S->top == NULL) error(ERR_INTERNAL, "Chyba realokacie!\n");
    memcpy(*S->top, add, sizeof(TTerm));
    S->top[1] = NULL;
    S->size++;
}

__attribute__((unused))
static TTerm *STop (PTSStack S) {
    return *S->top;
}

static TTerm *SPop (PTSStack S) {
    TTerm *ret = *S->top;
    *(S->top) = NULL;
    S->top--;
    S->size--;
    return ret;
}

static TTerm *SPick (PTSStack S, int offset) {
    //log("Picking Term on %d offset from %d/%d long stack\n", offset, 

//    EBP->value.ebp->size, ESP->value.esp->size);
    return S->term[S->size+offset];
}

static void SFree (PTSStack S) {
    while (*S->term != NULL) {
        free(*S->term);
        S->term++;
    }
    free(S);
}

/************************************
 *        Instrukcne funkcie        *
 ***********************************/

static void plus (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
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
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
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
}

static void mul (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            //log ("op1 %d, op2 %d\n", op1->value.integer, op2->value.integer);
            ret->value.integer = op1->value.integer * op2->value.integer;
        break;
        case TERM_REAL   :
            ret->value.real = op1->value.real * op2->value.real;
        break;
        default          : break;
    }
}

static void division (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;   
    switch (op1->type) {
        case TERM_INT    :
            if (op2->value.integer == 0)
                error(ERR_RNTM_ZERO, "Delenie 0 %d/%d\n", op1->value.integer, op2->value.integer);
            ret->value.integer = op1->value.integer / op2->value.integer;
            ret->type = op1->type;
        break;
        case TERM_REAL   :
            if (op2->value.real == 0)
                 error(ERR_RNTM_ZERO, "Delenie 0 %f/%f\n", op1->value.real, op2->value.real);
            ret->value.real = op1->value.real / op2->value.real;
            ret->type = op1->type;
        break;
        default          : break;
    }
}

static void assign (TTerm *op1, 
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.integer) : op1;
    if (ret == NULL) 
        ret = STop(EBP->value.ebp);
    else
        ret = ret->index ? SPick(EBP->value.ebp, ret->value.integer) : ret;
    ret->value = op1->value;
}

static void less (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
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

        default          : break;
    }
}

static void greater (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
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

        default          : break;
    }
}

static void lesseq (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
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

        default          : break;
    }
}

static void greateq (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
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

        default          : break;
    }
}

static void equal (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
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

        default          : break;
    }
}

static void nequal (TTerm *op1, TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
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

        default          : break;
    }
}

static void pushebp (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    TTerm EBPCPY = {
        .value.ebp = NULL
    };
    EBPCPY.value.ebp = malloc(sizeof(TTerm));
    memcpy(EBPCPY.value.ebp, EBP->value.ebp, sizeof(TSStack));
    SPush(ESP->value.esp, &EBPCPY);
    memcpy(EBP->value.ebp, ESP->value.esp, sizeof(TSStack));
}

static void popebp (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) { 
    memcpy(ESP->value.esp, EBP->value.ebp, sizeof(TSStack));
    size_t s = SPop(ESP->value.esp)->value.ebp->size;
    EBP->value.ebp->top  = s + ESP->value.esp->term;
    EBP->value.ebp->size = s;
    EBP->value.ebp->term = ESP->value.ebp->term;

}

static void ret (        TTerm *op1, TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
    // Local variable cleaning
    for (int i = 0; i < op1->value.integer; i++)
        free(SPop(ESP->value.esp));
   
    popebp(NULL, NULL, NULL);
    
    TTerm *add = SPop(ESP->value.esp);
    P3AC *pom = &EIP[add->value.address];
    free(add);
   // log("RETURN: on address:%u op1:%d op2:%d\n", (uint32_t)(pom-EIP), op1->value.integer, op2->value.integer);
    PEIP = pom;
    // Arguments cleaning
    for (int i = 0; i < op2->value.integer; i++)
        free(SPop(ESP->value.esp));
}

static void push (       TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    TTerm *s = malloc (sizeof(TTerm));
    if (s == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!\n");
    memcpy(s, op1, sizeof(TTerm));
    //log("PUSH: ESP: %u EBP:%u\n", ESP->value.esp->size, EBP->value.ebp->size);
    SPush(ESP->value.esp, s);
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
            ret->index = false;
            free(pom); 
        } else 
            error (ERR_INTERNAL, "Stack is empty");
    } else {
        SPop(ESP->value.esp);
    }
    //log ("POP\n");
}

static void jmp (        TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    PEIP = &EIP[op1->value.address-1];
    //log("JMP on adress %u with offset %u\n", (uint32_t)(PEIP-EIP), (*PEIP)->op);
}

static void jtrue (      TTerm *op1, TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    op2 = op2->index ? SPick(EBP->value.ebp, op2->value.offset) : op2;
 
    if (op1->value.boolean == true) {
        jmp(op2, NULL, NULL);
    }
        //log("Not jumping on adress %u with offset %u\n", (uint32_t)(PEIP-EIP), (*PEIP)->op);
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
    //log ("CALL: to address: %u\n", op1->value.address);
}

static void nop (
__attribute__ ((unused)) TTerm *op1,
__attribute__ ((unused)) TTerm *op2,
__attribute__ ((unused)) TTerm *ret) {
    /// NOPE NOPE NOPE
    //log("NOP");
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
  /*  op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;*/
 
    /// nacita zo zasobnika nti prvok
    TTerm *pom = SPick(EBP->value.ebp,op1->value.integer);
    if (pom == NULL) error(ERR_INTERNAL, "Load popol NULL, %d\n\
    ESP size %u, EBP size %u\n", op1->value.offset, (unsigned)ESP->value.esp->size, 
    (unsigned)EBP->value.ebp->size);
    memcpy(ret, pom, sizeof(TTerm));
}

static void load_param ( TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    TTerm *pom = SPick(BEBP->value.ebp,op1->value.integer);
    if (pom == NULL) error(ERR_INTERNAL, "Load popol NULL, %d\n\
    ESP size %u, EBP size %u\n", op1->value.offset, (unsigned)ESP->value.esp->size, 
    (unsigned)BEBP->value.ebp->size);
    memcpy(ret, pom, sizeof(TTerm));

}

static void store (      TTerm *op1,
__attribute__ ((unused)) TTerm *op2, TTerm *ret) {
    op1 = op1->index ? SPick(EBP->value.ebp, op1->value.offset) : op1;
    ret = ret->index ? SPick(EBP->value.ebp, ret->value.offset) : ret;
    /// ulozi na zasobnik na presne miesto data do termu
    memcpy(SPick(ESP->value.esp,ret->value.offset), op1, sizeof(TTerm));
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
    out->value.string = EMB_copy(str->value.string, from->value.integer, size->value.integer);
   // log ("%s", out->value.string);
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
    out->value.integer = EMB_find(str->value.string, fstr->value.string);
    ret(&zero, &one, NULL);
}

static void __write () {
    TTerm *n, k;
    int pocet = SPick(EBP->value.ebp,-2)->value.integer;
    for (int i = pocet; i > 0; i--) {
        n = SPick(EBP->value.ebp, -(2+i));
        switch (n->type) {
            case TERM_INT : 
                printf ("%d", n->value.integer);
            break;
            case TERM_REAL :
                printf ("%f", n->value.real);
            break;
            case TERM_STRING :
                printf ("%s", n->value.string);
            break;
            case TERM_BOOL :
                printf ("%s", n->value.boolean ? "true" : "false");
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
    ret(&zero, &one, NULL);
}


__attribute__ ((unused))
static void (*INST[])(TTerm *op1, TTerm *op2, TTerm *ret) = {
    &plus, &minus, &mul, &division, 
    &assign, &less, &greater, &lesseq, 
    &greateq, &equal, &nequal,  &call, &ret, 
    &push, &pop, &jtrue, &jmp, 
    &nop, &load, &load_param, &not, &store, 
    &pushebp, &popebp
    // TODO: Pridat dalsie funkcie!
};

TTerm embededFunc[6] = {
    {
        .value.emb_function = &__sort,
        .type = TERM_EMB,
        .name = "sort"
    },
    {
        .value.emb_function = &__copy,
        .type = TERM_EMB,
        .name = "copy" 
    },
    {
        .value.emb_function = &__length,
        .type = TERM_EMB,
        .name = "length" 
    },
    {
        .value.emb_function = &__find,
        .type = TERM_EMB,
        .name = "find" 
    }, 
    {
        .value.emb_function = &__write,
        .type = TERM_EMB,
        .name = "write" 
    }, 
    {
        .value.emb_function = &____readln,
        .type = TERM_EMB,
        .name = "readln" 
    }
};


void INT_interpret () {
    /// Instruction pointer
    PEIP = EIP;

    /// Stack pointer
    ESP->value.esp  = SInit ();
    EBP->value.ebp  = malloc (sizeof(TSStack));
    BEBP->value.ebp = malloc (sizeof(TSStack));
    // TODO: 
    //      * pridat volanie semantiky
    //      * pridat dealokacie: snad DONE
    //log("Runtime disasembly\n");
    for (int i = 0; *PEIP != NULL; i++) {
/*        printf("#%08u:\t%s\t%s%s%s%s%s\t{%d, %d, %d}\n",
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
*/

        INST[(*PEIP)->op]((*PEIP)->op1, (*PEIP)->op2, (*PEIP)->ret);


        PEIP++;
    }
    SFree(ESP->value.esp);
    return;
}
