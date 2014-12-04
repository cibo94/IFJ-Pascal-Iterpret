#!/usr/bin/env bash
export dir="test220"
export name="test220"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb -----------------------------}
{-------------------------- EXIT_CODE 2 --------------------------------------}
{ Definovanie funkcie s nazvom write                                          }

function write () : integer; 
begin
end;

begin
end.
"
export output=""
export retCode=0
