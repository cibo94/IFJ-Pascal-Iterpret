#!/usr/bin/env bash
export dir="FACTORIAL"
export name="FACTORiAL"
export file="../src/INT_interpret.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"
int main(int argc, char **argv) {
    FILE *f;
    if((f = fopen(argv[argc-1], "r")) == NULL) return -1;
    TStructLex *ret = malloc(sizeof(TStructLex));
    ret->lex = NULL;
    ret->type = 0;
    LEX_getLexem(ret, f);
    while(ret->type != TYPE_EOF) {
        printf("%s\n", ret->lex);
        LEX_getLexem(ret, f);
    }
    free(ret);
    fclose(f);
    return 0;
}'
export input="
var factorial:integer;

function fact(n:integer):integer;
var pom:integer;
begin
    if n = 0 then
      begin
        fact:=1
      end
    else
      begin
        pom:=n-1;
        pom:=fact(pom);
        fact:=pom*n
      end
end;

begin
    readln(factorial);
    factorial:=fact(factorial);
    write(factorial,'\\n')
end."
export output="var
factorial
:
integer
;
function
fact
(
n
:
integer
)
:
integer
;
var
pom
:
integer
;
begin
if
n
=
0
then
begin
fact
:=
1
end
else
begin
pom
:=
n
-
1
;
pom
:=
fact
(
pom
)
;
fact
:=
pom
*
n
end
end
;
begin
readln
(
factorial
)
;
factorial
:=
fact
(
factorial
)
;
write
(
factorial
,
\\n
)
end
.
"
export retCode=0
