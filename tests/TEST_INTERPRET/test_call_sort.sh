#!/usr/bin/env bash
export dir="CALL_SORT"
export name="CALL_SORT"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"

int main () {
    // VARIABLES GLOBAL, LABLES, TEMPORARY, CONSTANTS
    TTerm FOUR = {
        .value.integer = 4,
        .type = TERM_INT
    }, SORT = {
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
    EIP = malloc (sizeof(struct S3AC **)*12);
    struct S3AC inst[] = {
        { OP_PUSH,   &EAX,            NULL, NULL    },
        { OP_CALL,   &embededFunc[5], NULL, NULL    }, // READLINE(&STRING);
        { OP_PUSH,   &STRING,         NULL, NULL    }, 
        { OP_CALL,   &embededFunc[0], NULL, NULL    }, // COUNT:=STRLEN(STRING);
        { OP_POP,    NULL,            NULL, &SORT   },
        { OP_PUSH,   &NL,             NULL, NULL    },
        { OP_PUSH,   &STRING,         NULL, NULL    },
        { OP_PUSH,   &NL,             NULL, NULL    },
        { OP_PUSH,   &SORT ,          NULL, NULL    },
        { OP_PUSH,   &FOUR,           NULL, NULL    },
        { OP_CALL,   &embededFunc[4], NULL, NULL    }  // WRITE(COUNT, "\n", STRING, "\n");
    };
    for (int i = 0; i < 11; i++) {
        EIP[i] = &inst[i];
    }
    EIP[11] = NULL;
    INT_interpret ();
    free(EIP);
    return 0;
}'
export input="987654321"
export output="123456789
987654321
"
export retCode=0
