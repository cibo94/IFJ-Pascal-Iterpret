#!/usr/bin/env bash
export dir="test221"
export name="test221"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb -----------------------------}
{-------------------------- EXIT_CODE 2 --------------------------------------}
{ Chybajuca bodka za telom programu                                           }

begin
end
"
export output=""
export retCode=0
