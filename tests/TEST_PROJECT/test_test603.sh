#!/usr/bin/env bash
export dir="test603"
export name="test603"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{ Test interpretu - behova chyba nacitani ciselne hodnoty do real ze vstupu (navratovy kod 6) }
var
  i: real;
begin
  readln(i)
end.
"
export output=""
export retCode=0
