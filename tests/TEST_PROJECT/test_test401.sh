#!/usr/bin/env bash
export dir="test401"
export name="test401"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia semantickych chyb ------------------------------}
{-------------------------- EXIT_CODE 4 --------------------------------------}
{ Priradenie odlisnych datovych typov                                         }

var id : integer;
expr : real;

begin
	expr := 1.0;

	id := expr;
end.
"
export output=""
export retCode=0
