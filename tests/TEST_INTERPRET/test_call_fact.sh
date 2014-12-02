#!/usr/bin/env bash
export dir="CALL_FACT"
export name="CALL_FACT"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"

int main () {
    // VARIABLES GLOBAL, LABLES, TEMPORARY, CONSTANTS
    TTerm START = {
        .value.address = 13,
        .type = TERM_EIP
    }, ONE = {
        .value.integer = 1,
        .type  = TERM_INT
    }, ZERO = {
        .value.integer = 0,
        .type  = TERM_INT
    }, TWO = {
        .value.integer = 2,
        .type = TERM_INT
    }, ARG = ZERO,
    EAX = ZERO,
    RET = ONE,
    ELSE = {
        .value.address = 7,
        .type = TERM_EIP
    }, FACT = {
        .value.address = 1,
        .type = TERM_EIP
    }, NUM  = {
        .value.pointer = &RET,
        .type = TERM_INT
    }, NL = {
        .value.string = "\n",
        .type = TERM_STRING
    };
   /*************************************
    *           INSTRUCTIONS            *
    *************************************/ 
    EIP = malloc (sizeof(struct S3AC **)*23);
    struct S3AC inst[] = {
        { OP_JMP   , &START   , NULL , NULL }, //   GOTO __START                    0
        { OP_LOAD  , &ONE     , NULL , &ARG }, // FACT: ARG:=load(1);               1
        { OP_EQUAL , &ARG     , &ZERO, &EAX }, //       EAX:=(ARG==0);              2
        { OP_NOT   , &EAX     , NULL , &EAX }, //       EAX:=not(EAX);              3
        { OP_JTRUE , &EAX     , &ELSE, NULL }, //       JTRUE !(EAX == 0), ELSE     4
        { OP_ASSIGN, &ONE     , NULL , &RET }, //         RET:=1;                   5
        { OP_RET   , &ZERO    , &ONE , NULL }, //         RETURN;                   6
        { OP_MINUS , &ARG     , &ONE , &EAX }, // ELSE:                             7
        { OP_PUSH  , &EAX     , NULL , NULL }, //       EAX:=ARG-1;                 8
        { OP_CALL  , &FACT    , NULL , NULL }, //       FACT(EAX);                  9
        { OP_LOAD  , &ONE     , NULL , &ARG }, //       ARG:=load(1);              10 
        { OP_MUL   , &RET     , &ARG , &RET }, //       RET:=RET*ARG;              11
        { OP_RET   , &ZERO    , &ONE , NULL }, //       RETURN;                    12
        { OP_PUSH  , &NUM     , NULL , NULL }, // __START:                         13
        { OP_CALL  , &(embededFunc[5]), NULL , NULL }, 
                                               //   RET:=READLN(NUM);              14
        { OP_ASSIGN, &RET     , NULL , &NUM }, //   NUM:=RET                       15
        { OP_PUSH  , &NUM     , NULL , NULL }, //                                  16
        { OP_CALL  , &FACT    , NULL , NULL }, //   FACT(NUM);                     17
        { OP_PUSH  , &NL      , NULL , NULL }, //                                  18
        { OP_PUSH  , &RET     , NULL , NULL }, //                                  19
        { OP_PUSH  , &TWO     , NULL , NULL }, //                                  20
        { OP_CALL  , &(embededFunc[4]), NULL, NULL } 
                                               //   WRITE(2, RET, "\n");           21
    };
    for (int i = 0; i < 22; i++) {
        EIP[i] = &inst[i];
    }
    EIP[22] = NULL;
    INT_interpret ();
    print_EIP(EIP);
    free(EIP);
    return 0;
}'
export input="5"
export output="120
"
export retCode=0
