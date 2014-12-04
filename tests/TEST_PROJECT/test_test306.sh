#!/usr/bin/env bash
export dir="test306"
export name="test306"
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
{ Dopredna deklaracia funkcie bez nasledujucej definicie                      }

function func(param:integer) : integer; forward;

begin
end.
"
export output=""
export retCode=0
