
#!/usr/bin/env bash
export dir="FUNC"
export name="FUNC"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/inc.h"
int main(int argc, char **argv) {
    FILE *f = init(argv[argc-1]);
    SYN_parser(f);
    print_EIP(EIP);
    INT_interpret();
    return 0;
}'
export input="
var d:string;
{function reverse(a:integer):integer;
begin
    if a = 0
    then begin
        write(a, ''#10'')
    end
    else begin
        a:=a-1;
        a:=reverse(a);
        write(a, ''#10'');
        reverse:=a
    end
end;}

function doll():string;
begin
    doll:='So doll'
end;

begin
    d:=doll();
    write(d, ''#10'')
end."
export output=""
export retCode=0
