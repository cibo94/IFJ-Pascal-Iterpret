#!/usr/bin/env bash
export dir="test403"
export name="test403"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia semantickych chyb ------------------------------}
{-------------------------- EXIT_CODE 4 --------------------------------------}
{ Vyraz v cykle while nie je typu boolean                                     }

begin
	while 3.14 do
		begin
		end
end
"
export output=""
export retCode=0
