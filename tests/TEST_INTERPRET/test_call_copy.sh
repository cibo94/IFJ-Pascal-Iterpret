#!/usr/bin/env bash
export dir="CALL_COPY"
export name="CALL_COPY"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"

int main () {
    // VARIABLES GLOBAL, LABLES, TEMPORARY, CONSTANTS
    TTerm TWO = {
        .value.integer = 2,
        .type = TERM_INT
    }, SIZE = TWO,
    FROM = TWO,
    PSIZE = {
        .value.pointer = &SIZE,
        .type = TERM_INT
    }, PFROM = {
        .value.pointer = &FROM,
        .type = TERM_INT
    }, COPY = {
        .value.integer = 1,
        .type = TERM_STRING
    }, STRING  = {
        .value.integer = 0,
        .type = TERM_STRING
    }, EAX = {
        .value.pointer = &STRING,
        .type = TERM_STRING
    }, NL = {
        .value.string = "\n",
        .type = TERM_STRING
    };
   /*************************************
    *           INSTRUCTIONS            *
    *************************************/ 
    EIP = malloc (sizeof(struct S3AC **)*16);
    struct S3AC inst[] = {
        { OP_PUSH,   &EAX,            NULL, NULL    },
        { OP_CALL,   &embededFunc[5], NULL, NULL    }, // READLINE(&STRING);
        { OP_PUSH,   &PFROM,          NULL, NULL    },
        { OP_CALL,   &embededFunc[5], NULL, NULL    },
        { OP_PUSH,   &PSIZE,          NULL, NULL    },
        { OP_CALL,   &embededFunc[5], NULL, NULL    },
        { OP_PUSH,   &SIZE,           NULL, NULL    },
        { OP_PUSH,   &FROM,           NULL, NULL    },
        { OP_PUSH,   &STRING,         NULL, NULL    }, 
        { OP_CALL,   &embededFunc[1], NULL, NULL    }, // COUNT:=STRLEN(STRING);
        { OP_POP,    NULL,            NULL, &COPY   },
        { OP_PUSH,   &NL,             NULL, NULL    },
        { OP_PUSH,   &COPY,           NULL, NULL    },
        { OP_PUSH,   &TWO,            NULL, NULL    },
        { OP_CALL,   &embededFunc[4], NULL, NULL    }  // WRITE(COUNT, "\n", STRING, "\n");
    };
    for (int i = 0; i < 15; i++) {
        EIP[i] = &inst[i];
    }
    EIP[15] = NULL;
    INT_interpret ();
    free(EIP);
    free(STRING.value.string);
    free(COPY.value.string);
    return 0;
}'
export input="abcdefgh
2
6"
export output="bcdefg
"
export retCode=0
