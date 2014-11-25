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



#endif
