#!/usr/bin/env bash
export dir="FACTORIAL"
export name="FACTORiAL"
export file="../src/INT_interpret.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"

int main () {
    return 0;
}'
export input="
var factorial:integer;

function fact(n:integer):integer;
var pom:integer;
begin
    if n = 0 then
      begin
        fact:=1
      end
    else
      begin
        pom:=n-1;
        pom:=fact(pom);
        fact:=pom*n
      end
end;

begin
    readln(factorial);
    factorial:=fact(factorial);
    write(factorial,\"\\n\")
end.
"
export output=""
export retCode=0
