#!/usr/bin/env bash
export dir="test305"
export name="test305"
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
{ Dopredna deklaracia funkcie ma odlisnu hlavicku ako definicia               }

function func(diff_param:integer) : string; forward;

function func(param:integer) : string;
begin
end;


begin
end.
"
export output=""
export retCode=0
