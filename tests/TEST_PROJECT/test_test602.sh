#!/usr/bin/env bash
export dir="test602"
export name="test602"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{ Test interpretu - behova chyba nacitani dvou ciselnych hodnot do integeru ze vstupu (navratovy kod 6) }
var
  i: integer;
  j: integer;
begin
  readln(i);
  readln(j);
  write(i, ''#10'');
  write(j, ''#10'')
end.
"
export output="1
2"
export retCode=0
