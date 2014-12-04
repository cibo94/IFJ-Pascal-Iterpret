#!/usr/bin/env bash
export dir="test208"
export name="test208"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuci vyraz v podmienenom prikaze                                        }

begin
	if then
		begin
		end
	else
		begin
		end
end.
"
export output=""
export retCode=0
