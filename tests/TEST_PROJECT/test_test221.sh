#!/usr/bin/env bash
export dir="test221"
export name="test221"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb -----------------------------}
{-------------------------- EXIT_CODE 2 --------------------------------------}
{ Chybajuca bodka za telom programu                                           }

begin
end
"
export output=""
export retCode=0
