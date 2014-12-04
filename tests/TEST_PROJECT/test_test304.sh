#!/usr/bin/env bash
export dir="test304"
export name="test304"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia semantickych chyb ------------------------------}
{-------------------------- EXIT_CODE 3 --------------------------------------}
{ Dopredna deklaracia funkcie sa nachadza za definiciou                       }

function func(param:integer) : integer;
begin
end;

function func(param:integer) : integer; forward;

begin
end.
"
export output=""
export retCode=0
