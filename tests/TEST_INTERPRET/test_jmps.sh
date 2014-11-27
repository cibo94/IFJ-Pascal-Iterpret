#!/usr/bin/env bash
export dir="JMP"
export name="JUMP"
export file="../src/INT_interpret.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"

extern P3AC *EIP;

int main () {
    TTerm op1 = { 
        .value.address = 2,
        .type = TERM_INT,
        .name = "a"
    }, op2 = {
        .value.integer = 2,
        .type = TERM_INT,
        .name = "b"
    }, ret = {
        .value.integer = 2,
        .type = TERM_INT,
        .name = "c"
    };


    EIP = malloc (sizeof (P3AC)*6);
    
    struct S3AC s = (struct S3AC) {
        .op  = OP_JMP,
        .op1 = &op1,
        .op2 = NULL,
        .ret = NULL
    },s2 = (struct S3AC) {
        .op  = OP_MINUS,
        .op1 = &op1,
        .op2 = &ret,
        .ret = &op2             /// op2 by malo byt 0
    },s3 = (struct S3AC) {
        .op  = OP_MUL,
        .op1 = &op1,
        .op2 = &op2,
        .ret = &ret             /// AK prebehne skok tak ret bude 4 inak bude 0
    };
    EIP[0] = &s;
    EIP[1] = &s2;
    EIP[2] = &s3;
    EIP[3] = NULL;

    INT_interpret ();

    printf("Prebehol skok? %d == 4?\n", ret.value.integer);

    free(EIP);

    return 0;
}'
export input=""
export output=""
export retCode=0
