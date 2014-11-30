/**
 * \file INT_parser.h
 * \brief Interpret 3 adresneho kodu
 */

#ifndef h_INT_PAR
#define h_INT_PAR

#define EMBsort    embededFunc[0]
#define EMBcopy    embededFunc[1]
#define EMBlength  embededFunc[2]
#define EMBfind    embededFunc[3]
#define EMBwrite   embededFunc[4]
#define EMBreadln  embededFunc[5]

typedef enum {
    OP_PLUS, 
    OP_MINUS, 
    OP_MUL, 
    OP_DIV, 
    OP_ASSIGN, 
    OP_LESS,
    OP_GREAT,
    OP_LESSEQ,
    OP_GREATEQ, 
    OP_EQUAL,
    OP_NEQUAL,
    OP_CALL, 
    OP_RET, 
    OP_PUSH, 
    OP_POP, 
    OP_JTRUE, 
    OP_JMP, 
    OP_NOP, 
    OP_LOAD,
    OP_NOT,
    OP_STORE 
} E_OP;

typedef enum {
    TERM_INT,
    TERM_REAL,
    TERM_STRING,
    TERM_LABEL,
    TERM_BOOL,
    TERM_EIP,
    TERM_POINTER,
    TERM_OFFSET,
    TERM_EMB
} ETermType;

typedef struct S3AC *P3AC;

typedef struct STerm {
    union {
        int             integer;
        float           real;
        char           *string;
        bool            boolean;
        uint32_t        address;
        struct STerm   *pointer;
        uint32_t        offset;
        void          (*emb_function)();
    } value;
    ETermType type;
    char *name;
    bool index;
} TTerm;



struct S3AC {
    E_OP   op;
    TTerm *op1;
    TTerm *op2;
    TTerm *ret;
};


__attribute__ ((unused))
void INT_interpret ();
extern P3AC *EIP, *PEIP;
extern TTerm embededFunc[];
#endif
