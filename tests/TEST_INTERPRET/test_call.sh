#!/usr/bin/env bash
export dir="CALL"
export name="CALL"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"

extern P3AC *EIP;

int main () {
    PEIP = EIP = malloc (sizeof(P3AC)*42);
    TTerm EAX = {
              .value.address = 7,
              .type  = TERM_EIP,
              .index = false,
              .name  = "EAX"
          },
          EBX = {
              .value.offset  = 1,
              .type  = TERM_OFFSET,
              .index = true,
              .name  = "EBX"
          },
          ECX = {
              .value.offset = 3,
              .type  = TERM_OFFSET,
              .index = true,
              .name  = "ECX"
          },
          EDX = {
              .value.offset = 4,
              .type = TERM_OFFSET,
              .index = true,
              .name =  "EDX"
          },
          ALL = {
              .value.integer = 1,
              .type = TERM_INT,
              .index = false,
              .init = false,
              .name = "ALL"
          },
          FUN = {
              .value.address = 1,
              .type = TERM_EIP,
              .index = false,
              .name = "FUNC"
          },
          VAR = {
              .value.integer = 10,
              .type = TERM_INT,
              .index = false,
              .name = "VAR"
          };
    SEM_generate(OP_JMP,     &EAX,  NULL, NULL); //         jmp  __START;
    SEM_generate(OP_PUSH,    &ALL,  NULL, NULL); // __FUN:  push A;
    SEM_generate(OP_EBPPUSH, NULL,  NULL, NULL); //         push EBP; mov EBP, ESP;
    SEM_generate(OP_ASSIGN,  &ECX,  NULL, &EBX); //         mov EBX, ECX;
    SEM_generate(OP_MUL,     &ECX,  &EBX, &EDX); //         mul ECX, EBX; mov EDX, ECX;
    SEM_generate(OP_EBPPOP,  NULL,  NULL, NULL); //         mov ESP, EBP; pop EBP;
    SEM_generate(OP_RET,     &ALL,  &ALL, NULL); //         ret;
    SEM_generate(OP_PUSH,    &ALL,  NULL, NULL); // __START:push FUN;
    SEM_generate(OP_PUSH,    &VAR,  NULL, NULL); //         push VAR;
    SEM_generate(OP_CALL,    &FUN,  NULL, NULL); //         call FUN;
    SEM_generate(OP_POP,     NULL,  NULL, &VAR); //         pop VAR;
    print_EIP(EIP);
    INT_interpret();
    printf("%d\n", VAR.value.integer);
    free(EIP);
    return 0;
}'
export input=""
export output="Prebehol call? 8 == 8?
"
export retCode=0
