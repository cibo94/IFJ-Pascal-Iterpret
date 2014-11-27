#!/usr/bin/env bash
export dir="CALL_WITH_ARGS"
export name="CALL_WITH_ARGS"
export file="../src/INT_interpret.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"

extern P3AC *EIP;

int main () {
    // TERMS
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
    }, argc  = {
        .value.address = 2,
        .type = TERM_INT,
        .name = "ARGC"
    }, start = {
        .value.offset  = 5,
        .type = TERM_LABEL,
        .name = "__start"
    }, argv  = {
        .value.integer = 5,
        .type = TERM_INT,
        .name = "ARGV"
    };


    EIP = malloc (sizeof (P3AC)*11);

   /*************************************
    *           INSTRUCTIONS            *
    *************************************/ 

    struct S3AC s1 = (struct S3AC) {
        .op  = OP_JMP,
        .op1 = &start,
        .op2 = NULL,
        .ret = NULL
    },s2 = (struct S3AC) {
        .op  = OP_LOAD,
        .op1 = &argc,
        .op2 = NULL,
        .ret = &op1             /// op1 = 5
    },s3 = (struct S3AC) {      /// func
        .op  = OP_PLUS,
        .op1 = &op1,
        .op2 = &ret,
        .ret = &op2             /// op2 by malo byt 7
    },s4 = (struct S3AC) {
        .op  = OP_MUL,
        .op1 = &op1,
        .op2 = &op2,
        .ret = &ret             /// AK prebehne skok tak ret bude 35
    },s5 = (struct S3AC) {
        .op  = OP_RET,
        .op1 = &zero,
        .op2 = &argc,
        .ret = NULL
    },s6 = (struct S3AC) {      /// __start:
        .op  = OP_ASSIGN,
        .op1 = &op2,
        .op2 = NULL,
        .ret = &op1             /// 2
    },s7 = (struct S3AC) {
        .op  = OP_PUSH,
        .op1 = &argv,
        .op2 = NULL,
        .ret = NULL
    },s8 = (struct S3AC) {
        .op  = OP_PUSH,
        .op1 = &argc,
        .op2 = NULL,
        .ret = NULL
    },s9 = (struct S3AC) {
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
    EIP[6] = &s7;
    EIP[7] = &s8;
    EIP[8] = &s9;
    EIP[9] = NULL;


    INT_interpret ();

    printf("Prebehol call? %d == 35?\n", ret.value.integer);

    free(EIP);

    return 0;
}'
export input=""
export output="Prebehol call? 8 == 8?
"
export retCode=0
