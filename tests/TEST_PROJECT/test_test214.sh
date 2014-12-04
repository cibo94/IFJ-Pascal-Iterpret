#!/usr/bin/env bash
export dir="test214"
export name="test214"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Volanie funkcie write bez parametrov                                         }

begin
	write()
end.
"
export output=""
export retCode=0
