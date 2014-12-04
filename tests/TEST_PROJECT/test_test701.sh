#!/usr/bin/env bash
export dir="test701"
export name="test701"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{ Test interpretu - behova chyba neinicializovana promenna (navratovy kod 7) }
var
  i: integer;
begin
  write(i)
end.
"
export output=""
export retCode=0
