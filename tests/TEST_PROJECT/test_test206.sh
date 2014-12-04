#!/usr/bin/env bash
export dir="test206"
export name="test206"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Chybajuce telo funkcie, resp. forward                                        }

function func_again () : integer;

begin
end.
"
export output=""
export retCode=0
