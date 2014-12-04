#!/usr/bin/env bash
export dir="test705"
export name="test705"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{ Test prirazeni a opetovneho cteni z navratove hodnoty funkce  - free pascal to bere }
var
	a: integer;

function f(): integer;
var
	x: integer;
begin
	f:=10;
	x:=f;
	f:=x
end;

begin
	a:=f();
	write(a)
end.
"
export output="10"
export retCode=0
