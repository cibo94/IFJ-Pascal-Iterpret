#!/usr/bin/env bash
export dir="test12"
export name="test12"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{ Hledani korenu rovnice pomoci secen }

var
	x1: real;
	x0: real;
	h: real;

function fce(x: real): real;
begin
	fce := x*x*x + 3*x*x - 5*x - 2
end;

function r_abs(x: real): real;
begin
	if x < 0.0 then
	begin
		r_abs := x * (0-1)
	end
	else
	begin
		r_abs := x
	end
end;



{ Telo hlavniho programu }
begin
	h:=0.01;
	write('Zadejte pocatecni hodnotu: ');
	readln(x1);

	x0 := x1 - (fce(x1) * h) / ((fce(x1 + h) - fce(x1)));
	x1 := x0;

	while r_abs(fce(x0)) > h do
	begin
		x0 := x1 - (fce(x1) * h) / ((fce(x1 + h) - fce(x1)));
		x1 := x0
	end;

	write('Koren je: ', x0, ''#10'');
	write('Funkcni hodnota je: ', fce(x0), ''#10'')
end.
"
export output="Zadejte pocatecni hodnotu: Koren je: 1.4397
Funkcni hodnota je: 0.00382212"
export retCode=0
