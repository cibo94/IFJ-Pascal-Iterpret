#!/usr/bin/env bash
export dir="test203"
export name="test203"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{------------------- Detekcie syntaktickych chyb ------------------------------}
{ Definicna cast glob. premennych je uvodena len kluc. slovom var, bez ziadnej }
{ premennej.																																	 }

var

begin
end.
"
export output=""
export retCode=0
