#!/usr/bin/env bash
export dir="test11"
export name="test11"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{ Hledani korenu rovnice pomoci secen }

var
	x1: real;
	x2: real;
	x0: real;
	tmp1: real;
	tmp2: real;

function fce(x: real): real;
begin
	fce := x*x*x + 3*x*x - 5*x - 2
end;

function r_abs(x: real): real;
begin
	if x < 0.0 then
	begin
		r_abs := x * (0 - 1)
	end
	else
	begin
		r_abs := x
	end
end;

{ Telo hlavniho programu }
begin
	write('Zadej x1 a x2: ');
	readln(x1);
	readln(x2);

	if fce(x1) * fce(x2) < 0.0 then
	begin
		x0 := x1 - fce(x1) * (x2-x1) / (fce(x2) - fce(x1));
		{ write('x0 = ', x0, ''#10''); }

		if fce(x1) * fce(x0) < 0.0 then
		begin
			x2 := x0
		end
		else
		begin
			x1 := x0
		end

		while r_abs(fce(x0)) > 0.1 do
		begin
			x0 := x1 - fce(x1) * (x2-x1) / (fce(x2) - fce(x1));

			{ write('prubezny koren = ', x0, ''#10''); }
	
			if fce(x1) * fce(x0) < 0.0 then
			begin
				x2 := x0
			end
			else
			begin
				x1 := x0
			end
		end;

		write('koren je: ', x0, ''#10'');
		write('funkcni hodnota: ', fce(x0), '   '#10'')
	end
	else
	begin
		write('funkčni hodnoty maji stejna znamenka - nelze aplikovat metodu secen'#10'')
	end
end.
"
export output=""
export retCode=0
