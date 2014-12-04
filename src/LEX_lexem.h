/**
 * \file LEX_lexem.h
 * \brief Kniznica pracujuca s lexemami
 * \author 69DreamTeam
 */

#ifndef h_LEX_lexem
#define h_LEX_lexem


#define LEX_FLAGS_INIT              0x00000001      //!< premenna bola inicializovana
#define LEX_FLAGS_TYPE_BOOL         0x00000002      //!< funkcia vracia bool
#define LEX_FLAGS_TYPE_INT          0x00000004      //!< funkcia vracia int
#define LEX_FLAGS_TYPE_REAL         0x00000008      //!< funkcia vracia real
#define LEX_FLAGS_TYPE_STRING       0x00000010      //!< funkcia vracia string
#define LEX_FLAGS_TYPE_FUNCTION     0x00000020      //!< lexema je funkcia
#define LEX_FLAGS_TYPE_FUNC_DEK     0x00000040      //!< funkcia bola deklarovana
#define LEX_FLAGS_TYPE_FUNC_DEF     0x00000080      //!< funkcia bola definovana

#define STD_LNGTH 32                //!< pociatocna dlzka stringu


typedef enum {
    BASE,                           //!< Zakladny stav
    SLOVO,                          //!< Stav slovo
    OPER,                           //!< operator
    TERM_NUM,                       //!< cislo
    TERM_STR,                       //!< string
    SPACE,                          //!< medzery, nove riadky a pod
    COMMENT                         //!< komentare
} TEnumStatus;                      //!< status

typedef enum {
    START,                          //!< zakladny stav
    RETAZEC,                        //!< retazcovy stav
    MEDZI,                          //!< stav medzi uvodzovkami
    CISLO                           //!< stav za #
} TEnumLexStr;                      //!< stavovy enum pre LEX_str

typedef enum {
    KEY_BEGIN,
    KEY_BOOLEAN,
    KEY_DO,
    KEY_ELSE,
    KEY_END,
    KEY_FALSE,
//    KEY_FIND,
    KEY_FORWARD,
    KEY_FUNCTION,
    KEY_IF,
    KEY_INTEGER,
    KEY_READLN,
    KEY_REAL,
//    KEY_SORT,
    KEY_STRING,
    KEY_THEN,
    KEY_TRUE,
    KEY_VAR,
    KEY_WHILE,
    KEY_WRITE,
//    KEY_ARRAY,
//    KEY_REPEAT,
//    KEY_UNTIL,                      
    IDENTIFICATOR,                  //!< identifikator
    LBRACKET,                       //!< lava zatvorka        (
    RBRACKET,                       //!< prava zatvorka       )
    DDOT,                           //!< dvojbodka            :
    STREDNIK,                       //!< ukoncenie prikazu    ;
    CIARKA,                         //!< ciarka               ,
    BODKA,                          //!< bodka                .
    CONSTANT        = 1 << (sizeof(int)*8 -1),
    OP_RELATION     = 1 << (sizeof(int)*8 -2),
    OP_MATH         = 1 << (sizeof(int)*8 -3),
    OP_ASS          = 1 << (sizeof(int)*8 -4),
    INT_CONST       = 1 | CONSTANT, //!< cele cislo
    REAL_CONST      = 2 | CONSTANT, //!< desatine cislo
    STRING_CONST    = 3 | CONSTANT, //!< znakovy retazec
    OPERATOR_PLUS   = 1 | OP_MATH,  //!< operator plus        +
    OPERATOR_MINUS  = 2 | OP_MATH,  //!< operator minus       -
    OPERATOR_TIMES  = 3 | OP_MATH,  //!< operator krat        *
    OPERATOR_DIV    = 4 | OP_MATH,  //!< operator delene      /
    OPERATOR_ASSIGN = 1 | OP_ASS,   //!< operator rovna sa    :=
    OPERATOR_GREATER
                = 1 | OP_RELATION,  //!< operator vecsi       >
    OPERATOR_SMALLER
                = 2 | OP_RELATION,  //!< operator mensi       <
    OPERATOR_GREATEQ
                = 3 | OP_RELATION,  //!< operator vecsi rovny >=
    OPERATOR_SMALLEQ
                = 4 | OP_RELATION,  //!< operator mensi rovny <=
    OPERATOR_EQUAL
                = 5 | OP_RELATION,  //!< operator rovny       =
    OPERATOR_NEQUAL
                = 6 | OP_RELATION,  //!< operator nerovny     <>
    TYPE_EOF    = ~0                //!< koniec suboru
} TEnumLexem;                       //!< Enumerator ktory popisuje tym lexemy

typedef struct {
    uint32_t flags;                 //!< flagy pre blizsiu specifikaciu
    struct STerm *value;            //!< hodnota pri premennej, typ hodnoty urceny flagmi
    char *param;                    //!< index do zasobnika termov (interpret)/ pocet param. o funkcii
    char *lex;                      //!< Stringova representacia lexemy
    TEnumLexem type;                //!< druh lexemy
} TStructLex, *PTStructLex;         //!< struktura ktora obsahuje info o lexeme

typedef struct {
    bool realPart;                  //!< stav v casti za .
    bool expPart;                   //!< stav v casti za e
    bool partInit;                  //!< stav v casti ktora nebola inicializovana este
} *TStructNumStat, SNumStat;        //!< pomocna struktura pre lukasovu funkciu LEX_num

__attribute__ ((unused))
static char *KEY_WORDS[] = {
    "begin", "boolean", "do", "else", "end", "false", "forward",
    "function", "if", "integer", "readln", "real", "string",
    "then", "true", "var", "while", "write", 
//    "array", "repeat", "until", "sort", "find"
    NULL
};

extern unsigned int LINE_NUM;     //!< pozicia v subore podla riadkov

/** LEX_string
 * \brief Funkcia, ktora plni string nazvom lexemy
 * \param s ukazatel na string, ktory musi byt alokovany
 * \param ch vkladany char
 * \param poz posledny index
 * \see LEX_getLexem()
 */
void 
LEX_string(char     **s,
           int        ch,
           unsigned  *poz);

/** LEX_num
 * \brief Postupne vytvara ciselnu lexemu. Funkcia je volana v pripade, ze prvy znak novej lexemy je cislo (0..9)
 * \param c dalsi nacitany znak
 * \param s ukazatel na string
 * \param poz ukazatel na poziciu v stringu
 * \param NumStatus pomocna struktura na stavy
 * \param f subor (potrebny pre funkciu ungetc)
 * \return navratova hodnota, 0 ak bola lexema ukoncena, 1 ak je lexema nekompletna
 * \see LEX_getLexem()
 */
bool
LEX_num(int              c,
        char            **s, 
        unsigned        *poz, 
        TStructNumStat   NumStatus, 
        FILE            *f);

/** LEX_str
 * \brief Nacitava stringove retazce do lexemy
 * \param f File handle
 * \param s String do ktoreho tlaci znaky
 * \param ch Char ktory dostane
 * \param poz Pozicia v stringu
 * \param stav Stav v ktorom sa nachadza
 * \return TRUE ak skoncil ak nie tak FALSE
 * \see LEX_getLexem()
 */
bool
LEX_str(FILE        *f,
        char        **s,
        int          ch,
        unsigned    *poz,
        TEnumLexStr *stav);

/** LEX_operators
 * \brief Nacitanie operatorov
 * \param f File handle
 * \param Ret Vracana struktura
 * \param z Pismenko
 * \param i pozicia v stringu
 * \return TRUE ak skoncil ak nie tak FALSE
 * \see LEX_getLexem()
 */
bool
LEX_operators(FILE          *f, 
              TStructLex    *Ret, 
              int            z, 
              unsigned      *i);
/** LEX_ident
 * \brief Nacita identifikatorov do struktury
 * \param f File handle
 * \param s String do ktoreho tlaci znaky
 * \param z pismenko
 * \param i pozicia v stringu
 * \return TRUE ak skoncil ak nie tak FALSE
 * \see LEX_getLexem()
 */
int
LEX_ident(FILE          *f, 
          char         **s, 
          int            z, 
          unsigned      *i);
/** LEX_base
 * \brief Base stav
 * \param f File handle
 * \param NumStatus pomocna premenna pre LEX_num()
 * \param Ret Vracana struktura
 * \param z Pismenko
 * \param i index v stringu
 * \return TRUE ak skoncil ak nie tak FALSE
 * \see LEX_getLexem()
 */
int
LEX_base(FILE               *f, 
         TStructNumStat      NumStatus, 
         TEnumStatus        *state, 
         TStructLex         *Ret, 
         int                 z, 
         unsigned           *i);


/** LEX_getLexem
 * \brief Nacita lexemu do struktury
 * \param Ret Vracana struktura
 * \param f File handle
 * \return TRUE ak skoncil ak nie tak FALSE
 */
void
LEX_getLexem(PTStructLex Ret, 
             FILE       *f);

/** isKeyWord
 * \brief Skontroluje ci je identifikator klucove SLOVO a zmeni jeho typ
 * \param lex lexema
 * \return cislo/enum ak nasiel ak nie -1
 */
int
isKeyWord(PTStructLex lex);


#endif
