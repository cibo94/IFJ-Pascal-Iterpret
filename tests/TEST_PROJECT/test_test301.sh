#!/usr/bin/env bash
export dir="test301"
export name="test301"
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
{ Opakovana definicia premennej na glob. urovni                               }

var global : integer; 

function GLOBAL () : boolean;
begin
end;

begin
end.
"
export output=""
export retCode=0
