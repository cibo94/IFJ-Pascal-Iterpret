#!/usr/bin/env bash
export dir="test206"
export name="test206"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuce telo funkcie, resp. forward                                        }

function func_again () : integer;

begin
end.
"
export output=""
export retCode=0
