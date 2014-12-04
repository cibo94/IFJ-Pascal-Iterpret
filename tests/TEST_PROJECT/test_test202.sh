#!/usr/bin/env bash
export dir="test202"
export name="test202"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcie syntaktickych chyb ------------------------------}
{ Chybajuci strednik za definiciou glob. premennych                            }

var var1:integer;var2:boolean;var3:real;var4:string

begin
end.
"
export output=""
export retCode=0
