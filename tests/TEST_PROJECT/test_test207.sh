#!/usr/bin/env bash
export dir="test207"
export name="test207"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Zlozeny prikaz obsahuje len ';'                                              }

begin
	begin
		;
	end
end.
"
export output=""
export retCode=0
