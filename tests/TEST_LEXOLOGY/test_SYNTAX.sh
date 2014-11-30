export dir="SYNTAX"
export name="SYNTAX"
export file="../src/SYN_parser.c ../src/LEX_lexem.c "
export src='#include "../src/inc.h"
PGLOB_DEST pointers;
int main(int argc, char **argv) {
	FILE *f;
    if((f = fopen(argv[argc-1], "r")) == NULL) return -1;

    PTStructLex lex = malloc (sizeof (TStructLex));

   lex->lex = "f1";
   TSbinstrom pom;


    	SYN_parser(f);

     BS_Print(pointers->SYM_TABLE);
     //printf("%s\n",pointers->SCOPE->data->lex);
     pom=BS_Find(pointers->SYM_TABLE,lex);
     BS_Print(pom->loc_table);
    // BS_Free(pointers->SYM_TABLE);
     //BS_Print(pointers->SYM_TABLE);
    	fclose(f);

    }'




export input="var
a:integer;
b:real;
c:string;
d:boolean;
function f1(f1_p1: boolean; f1_p2:string):integer;
var f1_v1:integer;
	f1_v2:real;
	f1_v3:boolean;
begin

end;

function f2(lol: boolean; lola:string):integer;
var joj:integer;
	joj2:real;
	joj3:boolean;
begin
end;

begin
a:=1.4
end.

 "
  
export output="SYNTAX OK.
"
export retCode=0

