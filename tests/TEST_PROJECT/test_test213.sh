#!/usr/bin/env bash
export dir="test213"
export name="test213"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Volanie funkcie readln bez parametru                                         }

begin
	readln()
end.
"
export output=""
export retCode=0
