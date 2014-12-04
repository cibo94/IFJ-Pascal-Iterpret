#!/usr/bin/env bash
export dir="test703"
export name="test703"
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
	x:integer;

function f():integer;
var
	a:integer;
begin
  	a := 1 * 2 * 3 * 4
end;

begin
	x:=f()
end.
"
export output=""
export retCode=0
