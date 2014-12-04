#!/usr/bin/env bash
export dir="test801"
export name="test801"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{ Test interpretu - behova chyba deleni nulou (navratovy kod 8) }
var
  i: integer;
  j: integer;
begin
  i:=42;
  j:=0;
  write(i/j)
end.
"
export output=""
export retCode=0
