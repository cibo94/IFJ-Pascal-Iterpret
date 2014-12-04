#!/usr/bin/env bash
export dir="test303"
export name="test303"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia semantickych chyb ------------------------------}
{-------------------------- EXIT_CODE 3 --------------------------------------}
{ Opakovana definicia funkcie                                                 }

function EMPTY () : integer;
begin
end;

function empty():integer;
begin
end;

begin
end.
"
export output=""
export retCode=0
