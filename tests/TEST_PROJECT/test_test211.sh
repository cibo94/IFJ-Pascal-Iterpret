#!/usr/bin/env bash
export dir="test211"
export name="test211"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuci zlozeny prikaz za 'do' v prikaze cyklu while                       }

begin
	while false do
end.
"
export output=""
export retCode=0
