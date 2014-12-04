#!/usr/bin/env bash
export dir="test212"
export name="test212"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Viac parametrov vo volani funkcie readln                                     }

var readln_var : string;

begin
	readln(readln_var, readln_var)
end.
"
export output=""
export retCode=0
