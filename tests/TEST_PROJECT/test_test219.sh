#!/usr/bin/env bash
export dir="test219"
export name="test219"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb -----------------------------}
{-------------------------- EXIT_CODE 2 --------------------------------------}
{ Pouzite klucoveho slova pre vstavanu funkciu ako premennu                   }

var write : integer;

begin
end.
"
export output=""
export retCode=0
