#!/usr/bin/env bash
export dir="test217"
export name="test217"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb -----------------------------}
{-------------------------- EXIT_CODE 2 --------------------------------------}
{ Za prikazom nasleduju 2 stredniky                                           }

var id : real;

begin
	id := 1.0;;
	id := id
end.
"
export output=""
export retCode=0
