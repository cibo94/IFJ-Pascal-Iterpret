#!/usr/bin/env bash
export dir="test216"
export name="test216"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb -----------------------------}
{-------------------------- EXIT_CODE 2 --------------------------------------}
{ Lavy operand nie je l-hodnota                                               }

begin
	1.0 := 1.0
end.
"
export output=""
export retCode=0
