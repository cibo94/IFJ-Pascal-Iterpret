#!/usr/bin/env bash
export dir="CALL_FIND"
export name="CALL_FIND"
export file="../src/INT_interpret.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"

int main () {
    // VARIABLES GLOBAL, LABLES, TEMPORARY, CONSTANTS
    TTerm TWO = {
        .value.integer = 2,
        .type = TERM_INT
    }, FIND = TWO,
    PFIND = {
        .value.pointer = &FIND,
        .type = TERM_STRING
    }, INDEX = {
        .value.integer = 1,
        .type = TERM_INT
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
    EIP = malloc (sizeof(struct S3AC **)*13);
    struct S3AC inst[] = {
        { OP_PUSH,   &EAX,            NULL, NULL    },
        { OP_CALL,   &embededFunc[5], NULL, NULL    }, // READLINE(&STRING);
        { OP_PUSH,   &PFIND,          NULL, NULL    },
        { OP_CALL,   &embededFunc[5], NULL, NULL    },
        { OP_PUSH,   &FIND,           NULL, NULL    },
        { OP_PUSH,   &STRING,         NULL, NULL    }, 
        { OP_CALL,   &embededFunc[3], NULL, NULL    }, // COUNT:=STRLEN(STRING);
        { OP_POP,    NULL,            NULL, &INDEX  },
        { OP_PUSH,   &NL,             NULL, NULL    },
        { OP_PUSH,   &INDEX,          NULL, NULL    },
        { OP_PUSH,   &TWO,            NULL, NULL    },
        { OP_CALL,   &embededFunc[4], NULL, NULL    }  // WRITE(COUNT, "\n", STRING, "\n");
    };
    for (int i = 0; i < 12; i++) {
        EIP[i] = &inst[i];
    }
    EIP[12] = NULL;
    INT_interpret ();
    free(EIP);
    free(STRING.value.string);
    free(FIND.value.string);
    return 0;
}'
export input="abcdefgh
efgh"
export output="5
"
export retCode=0
