#!/usr/bin/env bash
export dir="prog1"
export name="prog1"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{ Program 1: Vypocet faktorialu (iterativne) }
var
  a : integer;
  vysl : integer;
begin
  write('Zadejte cislo pro vypocet faktorialu: ');
  readln(a);
  if a < 0 then
  begin
    write('Faktorial nelze spocitat'#10'')
  end
  else
  begin
    vysl := 1;
    while a > 0 do
    begin
      vysl := vysl * a;
      a := a - 1
    end;
    write('Vysledek je: ', vysl, ''#10'')
  end
end.
"
export output=""
export retCode=0
