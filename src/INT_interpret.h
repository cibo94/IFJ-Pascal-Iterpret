/**
 * \file INT_interpret.h
 * \brief Interpret 3 adresneho kodu
 * \author Miroslav Cibulka
 */

#ifndef h_INT_PAR
#define h_INT_PAR

#define EMBsort    embededFunc[0]  //!< zabudovana funkcia na sort retazca
#define EMBcopy    embededFunc[1]  //!< zabudovana funkcia na kopirovanie retazca
#define EMBlength  embededFunc[2]  //!< zabudovana funkcia na dlzku retazca
#define EMBfind    embededFunc[3]  //!< zabudovana funkcia na hladania podretazca v retazci
#define EMBwrite   embededFunc[4]  //!< zabudovana funkcia na vypis
#define EMBreadln  embededFunc[5]  //!< zabudovana funkcia na nacitanie

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
} E_OP;                 //!< urcuje typ operatora

typedef enum {
    TERM_INT,
    TERM_REAL,
    TERM_STRING,
    TERM_BOOL,
    TERM_EIP,
    TERM_POINTER,
    TERM_OFFSET,
    TERM_EMB,
    TERM_EBP,
    TERM_ESP
} ETermType;            //!< urcuje typ TERMu

typedef struct S3AC *P3AC;  //!< pole 3adresnych kodov == samotne prikazy
typedef struct SStack TSStack, *PTSStack;

typedef struct STerm {
    union {
        int             integer;         //!< Ak je term typ TERM_INT operacie pracuju s integerom
        float           real;            //!< Ak je term typ TERM_REAL operacie pracuju s realnym cislom
        char           *string;          //!< Ak je term tyo TERM_STRING operacie pracuju s ukazatelom na string
        bool            boolean;         //!< Ak je term typ TERM_BOOL operacie pouzivaju hodnotu ako boolean
        uint32_t        address;         //!< Adresa do pola instrukcii TERM_EIP
        struct STerm   *pointer;         //!< Ukazatel na iny Term -> koli readln, zo zadania nemame riesit TERM_POINTER
        uint32_t        offset;          //!< Offset do zasobnika TERM_OFFSET
        void          (*emb_function)(); //!< ukazatel na zabudovanu funkciu tyo TERM_EMB
        PTSStack        esp;
        PTSStack        ebp;
    } value;                //!< hodnota termu
    ETermType type;         //!< typ termu
    char *name;             //!< meno termu ???treba???
    bool index;             //!< Pre lukasa -> poradie premennej/param vo funkcii/glob
    bool init;              //!< DEFINUJE CI JE PREMENNA INICIALIZOVANA
} TTerm;                    //!< Term -> struktura premennej

struct SStack {
    TTerm     **term;       //!< TERM
    TTerm     **top;        //!< Top term na zasobniku
    size_t      size;       //!< size of stack
};

struct S3AC {
    E_OP   op;              //!< Operacia medzi operatormi
    TTerm *op1;             //!< Operator 1
    TTerm *op2;             //!< Operator 2
    TTerm *ret;             //!< Navratova premenna
};                          //!< Instrukcia


__attribute__ ((unused))
void INT_interpret ();      //!< zakladna funkcia -> spusti interpret, musi byt vyplneny EIP a ukonceny NULLom
extern P3AC *EIP,           //!< EIP -> globalne pole intrukcii
            *PEIP;          //!< PEIP -> ukazatel do pola instrukcii -> koli skokom a pod
extern TTerm embededFunc[]; //!< zabudovane funkcie -> readln, write, sort, length, ...
#endif
