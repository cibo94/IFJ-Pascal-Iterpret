#!/usr/bin/env bash
export dir="test10"
export name="test10"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{ CIFSUCET.PAS              Copyright (c) fake man, gazda					  }
{ Program urèí ciferný súèet n-èísel plus e¹te nieèo k tomu        	}
{                                                                   }
{ Author: fake man, gazda                                           }
{ Date  : 15.12.2010                           http://www.trsek.com }

var a:integer;  b:integer; n:integer; i:integer; sign:integer; wait:integer;

function gdiv(a : integer; b : integer) : integer;
var sign : integer; 
begin
	if a * b < 0 then
		begin
			sign := 0 - 1
		end
	else
		begin
			sign := 1
		end;
	gdiv := sign
end;

begin
  write('Zadaj 1. cislo pre urcenie znamienka vysledku sucinu cisel: '); 
	readln(a);
	write('Zadaj 2. cislo pre urcenie znamienka vysledku sucinu cisel: '); 
	readln(b);
	write('Zadaj pocet opakovani: ');
	readln(n);

	i := 1;
	while i <= n do
		begin 
			sign := gdiv(a, b);
			readln(wait)
		end;

	write('Znamienko je ',sign,''#10'')
end.
"
export output=""
export retCode=0
