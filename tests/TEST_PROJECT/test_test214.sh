#!/usr/bin/env bash
export dir="test214"
export name="test214"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Volanie funkcie write bez parametrov                                         }

begin
	write()
end.
"
export output=""
export retCode=0
