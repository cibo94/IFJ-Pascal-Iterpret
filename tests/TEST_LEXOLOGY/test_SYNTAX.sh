export dir="SYNTAX"
export name="SYNTAX"
export file="../src/SYN_parser.c ../src/LEX_lexem.c "
export src='#include "../src/inc.h"
PGLOB_DEST pointers;
int main(int argc, char **argv) {
	FILE *f;
    if((f = fopen(argv[argc-1], "r")) == NULL) return -1;

    //PTStructLex lex = malloc (sizeof (TStructLex));
    //lex->lex = "f1";
    //TSbinstrom pom;


    	SYN_parser(f);
     BS_Print(pointers->SYM_TABLE);


    //printf("%s\n",pointers->SCOPE->data->lex);
    //pom=BS_Find(pointers->SYM_TABLE,lex);
    //BS_Print(pom->loc_table);
    //BS_Free(pointers->SYM_TABLE);
    //BS_Print(pointers->SYM_TABLE);
    
    	fclose(f);
    }'




export input="{Program 2: Vypocet faktorialu ( rekurzivne ) }
var
a :integer;
vysl :integer;

function factorial(n :integer; i :integer) :integer; forward;
{Definice funkce pro vypocet hodnoty  faktorialu}
function factorial(n :integer; i :real) :integer;
var
temp_result :integer;
decremented_n :integer;
begin
if n < 2 then begin
factorial := 1
end
else
begin
decremented_n := n - 1;
temp_result := factorial(decremented_n);
factorial := n * temp_result
end
end;
{Hlavni telo programu }
begin
write('Zadejte cislo  pro vypocet faktorialu : ');
readln(a);
if a < 0 then
{Pokracovani hlavniho tela programu }
begin
write('Faktorial nelze spocitat '#10'')
end
else
begin
vysl := factorial(a);
write('Vysledek je :', vysl, ''#10'')
end
end.
 "
  
export output="SYNTAX OK.
"
export retCode=0

