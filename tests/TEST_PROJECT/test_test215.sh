#!/usr/bin/env bash
export dir="test215"
export name="test215"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Ciarka za poslednym parametrom funkcie write                                 }

begin
	write(true, 1, 1.0, 'tralala',)
end.
"
export output=""
export retCode=0
