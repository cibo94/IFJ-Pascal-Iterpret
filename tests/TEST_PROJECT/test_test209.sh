#!/usr/bin/env bash
export dir="test209"
export name="test209"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuci zlozeny prikaz za 'then' v podmienenom prikaze                     }

begin
	if true then
	else
		begin
		end
end.
"
export output=""
export retCode=0
