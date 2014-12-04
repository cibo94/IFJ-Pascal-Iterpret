#!/usr/bin/env bash
export dir="test209"
export name="test209"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuci zlozeny prikaz za 'then' v podmienenom prikaze                     }

begin
	if true then
	else
		begin
		end
end.
"
export output=""
export retCode=0
