#!/usr/bin/env bash
export dir="test302"
export name="test302"
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
{ Opakovana definicia premennej na lok. urovni                                }

function EMPTY () : integer;
var eMpTy : integer;
begin
end;

begin
end.
"
export output=""
export retCode=0
