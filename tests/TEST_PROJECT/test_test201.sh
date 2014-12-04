#!/usr/bin/env bash
export dir="test201"
export name="test201"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------------- Syntakticke testy ----------------------------------}
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Testovanie na odhalenie nespravnej postupnosti sekcii programu							 }
{ Definicia uzivatelskych funkcii vymenena s deklaraciou glob. premennych      }

function my_func() : boolean;
var local_var : boolean;
begin
end;

var global_var : boolean;

begin
end.
"
export output=""
export retCode=0
