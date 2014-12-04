#!/usr/bin/env bash
export dir="test204"
export name="test204"
export file="../src/LEX_lexem.c"
export src='#include "../src/../src/inc.h"

int main () {
    return 0;
}'
export input="
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Za poslednym parametrom funkcie sa nachadza bodkociarka                      }

function test_func (p_1:integer;p_2:boolean;p_3:real;p_4:string;) : string;
begin
end;

begin
end.
"
export output=""
export retCode=0
