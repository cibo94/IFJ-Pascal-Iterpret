#!/usr/bin/env bash
export dir="CALL_FACT"
export name="CALL_FACT"
export file="../src/INT_interpret.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"

extern P3AC *EIP;

int main () {
    // VARIABLES GLOBAL, LABLES, TEMPORARY, CONSTANTS
    TTerm START = {
        .value.address = 13,
        .type = TERM_LABEL
    }, ONE = {
        .value.integer = 1,
        .type  = TERM_INT
    }, ZERO = {
        .value.integer = 0,
        .type  = TERM_INT
    }, ARG = ZERO,
    EAX = ZERO,
    RET = ONE,
    ELSE = {
        .value.address = 7,
        .type = TERM_LABEL
    }, FACT = {
        .value.address = 1,
        .type = TERM_LABEL
    }, NUM  = {
        .value.integer = 10,
        .type = TERM_INT
    };
   /*************************************
    *           INSTRUCTIONS            *
    *************************************/ 
    EIP = malloc (sizeof(struct S3AC **)*16);
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
        { OP_CALL  , &FACT    , NULL , NULL }  //   FACT(NUM);                     14
    };
    for (int i = 0; i < 15; i++) {
        EIP[i] = &inst[i];
    }
    EIP[15] = NULL;
    INT_interpret ();
    printf("Faktorial z %d = %d?\n", NUM.value.integer, RET.value.integer);
    free(EIP);
    return 0;
}'
export input=""
export output="Prebehol call? 4 == 4?
"
export retCode=0
