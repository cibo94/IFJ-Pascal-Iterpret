#!/usr/bin/env bash
export dir="test404"
export name="test404"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
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
