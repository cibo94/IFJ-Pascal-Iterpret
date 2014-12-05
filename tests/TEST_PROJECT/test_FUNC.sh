
#!/usr/bin/env bash
export dir="FUNC"
export name="FUNC"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/inc.h"
int main(int argc, char **argv) {
    FILE *f = init(argv[argc-1]);
    SYN_parser(f);
//   print_EIP(EIP);
    INT_interpret();
    return 0;
}'
export input="
var glob:integer;


function doll(d:integer):integer;
begin
    if d = 0
    then begin
        write(d, ''#10'')
    end else begin
        d:=d-1;
        d:=doll(d);
        write(d, ''#10'')
    end;
    doll:=d+1
end;

begin
    glob:=10;
    glob:=doll(glob)
end."
export output=""
export retCode=0
