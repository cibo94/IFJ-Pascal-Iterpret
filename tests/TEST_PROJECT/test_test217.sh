#!/usr/bin/env bash
export dir="test217"
export name="test217"
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
{ Za prikazom nasleduju 2 stredniky                                           }

var id : real;

begin
	id := 1.0;;
	id := id
end.
"
export output=""
export retCode=0
