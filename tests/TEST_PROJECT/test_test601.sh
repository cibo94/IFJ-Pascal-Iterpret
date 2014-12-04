#!/usr/bin/env bash
export dir="test601"
export name="test601"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{ Test interpretu - behova chyba nacitani ciselne hodnoty do integeru ze vstupu (navratovy kod 6) }
var
  i: integer;
begin
  readln(i)
end.
"
export output=""
export retCode=0
