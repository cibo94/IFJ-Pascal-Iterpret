#!/usr/bin/env bash
export dir="test205"
export name="test205"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuci strednik za definiciou funkcie                                     }

function our_func () : real;
begin
end

begin
end.
"
export output=""
export retCode=0
