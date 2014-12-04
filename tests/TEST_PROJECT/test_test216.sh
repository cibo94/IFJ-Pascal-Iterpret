#!/usr/bin/env bash
export dir="test216"
export name="test216"
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
{ Lavy operand nie je l-hodnota                                               }

begin
	1.0 := 1.0
end.
"
export output=""
export retCode=0
