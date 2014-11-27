/**
 * \file INT_parser.h
 * \brief Interpret 3 adresneho kodu
 */

#ifndef h_INT_PAR
#define h_INT_PAR

// DUMMY typedefs:

typedef struct S3ADD *P3ADD;


// DUMMY functions:

///////////////////////


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

typedef enum {
    TERM_INT,
    TERM_REAL,
    TERM_STRING,
    TERM_LABEL,
    TERM_BOOL,
    TERM_EIP,
    TERM_POINTER,
    TERM_OFFSET
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
    } value;
    ETermType type;
    char *name;
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
#endif
