#!/usr/bin/env bash
export dir="CALL"
export name="CALL"
export file="../src/INT_interpret.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"

extern P3AC *EIP;

int main () {
    TTerm op1 = { 
        .value.integer = 0,
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
    }, label = {
        .value.address = 1,
        .type = TERM_LABEL,
        .name = "func"
    }, zero  = {
        .value.integer = 0,
        .type = TERM_INT,
        .name = "Zero"
    }, start = {
        .value.offset = 4,
        .type = TERM_LABEL,
        .name = "__start"
    };


    EIP = malloc (sizeof (P3AC)*11);
    
    struct S3AC s1 = (struct S3AC) {
        .op  = OP_JMP,
        .op1 = &start,
        .op2 = NULL,
        .ret = NULL
    },s2 = (struct S3AC) {      /// func
        .op  = OP_PLUS,
        .op1 = &op1,
        .op2 = &ret,
        .ret = &op2             /// op2 by malo byt 4
    },s3 = (struct S3AC) {
        .op  = OP_MUL,
        .op1 = &op1,
        .op2 = &op2,
        .ret = &ret             /// AK prebehne skok tak ret bude 8 inak bude ???
    },s4 = (struct S3AC) {
        .op  = OP_RET,
        .op1 = &zero,
        .op2 = &zero,
        .ret = NULL
    },s5 = (struct S3AC) {      /// __start:
        .op  = OP_ASSIGN,
        .op1 = &op2,
        .op2 = NULL,
        .ret = &op1             /// 2
    },s6 = (struct S3AC) {
        .op  = OP_CALL,
        .op1 = &label,
        .op2 = NULL,
        .ret = NULL
    };
    EIP[0] = &s1;
    EIP[1] = &s2;
    EIP[2] = &s3;
    EIP[3] = &s4;
    EIP[4] = &s5;
    EIP[5] = &s6;
    EIP[6] = NULL;

    INT_interpret ();

    printf("Prebehol call? %d == 8?\n", ret.value.integer);

    free(EIP);

    return 0;
}'
export input=""
export output="Prebehol call? 8 == 8?
"
export retCode=0
