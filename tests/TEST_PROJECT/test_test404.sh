#!/usr/bin/env bash
export dir="test404"
export name="test404"
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
{ Parameter funkcie readln je typu boolean.                                   }

var bo : boolean;

begin
	readln(bo);
end.
"
export output=""
export retCode=0
