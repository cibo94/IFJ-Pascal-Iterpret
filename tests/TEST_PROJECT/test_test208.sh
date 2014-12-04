#!/usr/bin/env bash
export dir="test208"
export name="test208"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuci vyraz v podmienenom prikaze                                        }

begin
	if then
		begin
		end
	else
		begin
		end
end.
"
export output=""
export retCode=0
