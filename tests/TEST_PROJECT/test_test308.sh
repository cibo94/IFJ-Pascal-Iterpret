#!/usr/bin/env bash
export dir="test308"
export name="test308"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia semantickych chyb ------------------------------}
{-------------------------- EXIT_CODE 3 --------------------------------------}
{ Redefinovanie vstavanej funkcie find                                        }

function find (s : string; search : string) : integer;
begin
end;

begin
end.
"
export output=""
export retCode=0
