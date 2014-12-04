#!/usr/bin/env bash
export dir="test702"
export name="test702"
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
  	f:=3*4*8+1345 + a
end;

begin
	x:=f()
end.
"
export output=""
export retCode=0
