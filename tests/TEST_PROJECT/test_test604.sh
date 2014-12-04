#!/usr/bin/env bash
export dir="test604"
export name="test604"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{ Test interpretu - behova chyba nacitani dvou ciselnych hodnot do real ze vstupu (navratovy kod 6) }
var
  i: real;
  j: real;
begin
  readln(i);
  readln(j);
  write(i, ''#10'');
  write(j, ''#10'')
end.
"
export output="3.14
42"
export retCode=0
