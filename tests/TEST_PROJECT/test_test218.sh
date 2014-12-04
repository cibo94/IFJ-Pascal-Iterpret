#!/usr/bin/env bash
export dir="test218"
export name="test218"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb -----------------------------}
{-------------------------- EXIT_CODE 2 --------------------------------------}
{ Prazdny program                                                             }
"
export output=""
export retCode=0
