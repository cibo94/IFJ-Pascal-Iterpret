#!/usr/bin/env bash
export dir="test213"
export name="test213"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Volanie funkcie readln bez parametru                                         }

begin
	readln()
end.
"
export output=""
export retCode=0
