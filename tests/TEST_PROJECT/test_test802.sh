#!/usr/bin/env bash
export dir="test802"
export name="test802"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{ Test interpretu - behova chyba deleni nulou (navratovy kod 8) }
var
  i: real;
  j: real;
begin
  i:=3.14;
  j:=0.0;
  write(i/j)
end.
"
export output=""
export retCode=0
