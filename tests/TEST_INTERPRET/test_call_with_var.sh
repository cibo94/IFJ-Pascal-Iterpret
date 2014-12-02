#!/usr/bin/env bash
export dir="CALL_WITH_VAR"
export name="CALL_WITH_VAR"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"

extern P3AC *EIP;

int main () {
    // TERMS
    TTerm reg1 = {
        .value.address = 1
    }, reg2    = {
        .value.address = 0
    }, pom     = { 
        .value.integer = 0
    }, ret = {
        .value.integer = 2,
        .type = TERM_INT,
        .name = "c"
    }, label = {
        .value.address = 1,
        .type = TERM_EIP,
        .name = "func"
    }, zero  = {
        .value.integer = 0,
        .type = TERM_INT,
        .name = "Zero"
    }, start = {
        .value.offset  = 8,
        .type = TERM_EIP,
        .name = "__start"
    }, loc   = {
        .value.integer = 2,
        .type = TERM_INT,
        .name = "LocVar"
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
        .op  = OP_PUSH,
        .op1 = &ret,
        .op2 = NULL,
        .ret = NULL             /// reg1 = 5
    },s3 = (struct S3AC) {      /// func
        .op  = OP_PUSH,
        .op1 = &ret,
        .op2 = NULL,
        .ret = NULL             /// reg2 by malo byt 7
    },s4 = (struct S3AC) {
        .op  = OP_LOAD,
        .op1 = &reg2,
        .op2 = NULL,
        .ret = &reg2              /// AK prebehne skok tak ret bude 35
    },s5 = (struct S3AC) {
        .op  = OP_LOAD,
        .op1 = &reg1,
        .op2 = NULL,
        .ret = &reg1
    },s6 = (struct S3AC) {
        .op  = OP_MUL,
        .op1 = &reg1,
        .op2 = &reg2,
        .ret = &pom
    },s7 = (struct S3AC) {
        .op  = OP_ASSIGN,
        .op1 = &pom,
        .op2 = NULL,
        .ret = &ret
    },s8 = (struct S3AC) {
        .op  = OP_RET,
        .op1 = &loc,
        .op2 = &zero,
        .ret = NULL
    },s9 = (struct S3AC) {         /// __start:
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

    printf("Prebehol call? %d == 4?\n", pom.value.integer);

    free(EIP);

    return 0;
}'
export input=""
export output="Prebehol call? 4 == 4?
"
export retCode=0
