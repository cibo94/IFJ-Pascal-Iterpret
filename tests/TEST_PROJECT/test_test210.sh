#!/usr/bin/env bash
export dir="test210"
export name="test210"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuci vyraz v prikaze cyklu                                              }

begin	
	while do
		begin
		end
end.
"
export output=""
export retCode=0
