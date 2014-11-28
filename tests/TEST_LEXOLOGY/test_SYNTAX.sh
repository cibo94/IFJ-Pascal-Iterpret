export dir="SYNTAX"
export name="SYNTAX"
export file="../src/SYN_parser.c ../src/LEX_lexem.c ../src/SEM.c  "
export src='#include "../src/inc.h"
PGLOB_DEST pointers;
int main(int argc, char **argv) {
	FILE *f;
    if((f = fopen(argv[argc-1], "r")) == NULL) return -1;


    	SYN_parser(f);

      BS_Print(pointers->SYM_TABLE);

    	fclose(f);

    }'
export input="Var   disks: integer;
      ahoj: string;
      noha:integer;

function test():integer;
begin
test:=1
end;

function test2(p1:string; p2:integer; p3:boolean; p4:real): integer;
begin
test2:=p1+p2+4;
end;


 

 begin
 end.
 "
  
export output="SYNTAX OK.
"
export retCode=0