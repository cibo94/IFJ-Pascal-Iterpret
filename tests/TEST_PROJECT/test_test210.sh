#!/usr/bin/env bash
export dir="test210"
export name="test210"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuci vyraz v prikaze cyklu                                              }

begin	
	while do
		begin
		end
end.
"
export output=""
export retCode=0
