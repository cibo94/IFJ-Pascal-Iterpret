export dir="SYNTAX"
export name="SYNTAX"
export file="../src/SYN_parser.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"
int main(int argc, char **argv) {
	FILE *f;
    if((f = fopen(argv[argc-1], "r")) == NULL) return -1;

    	SYN_parser(f);

    	fclose(f);




    }'
export input="Var
   disks:   integer;
 function Hanoi(source:integer):boolean;

   begin
   if n > 0 then
      begin
      disks:=Hanoi(source, destination, temp, disks);
      write('Move disk from peg ',source,' to peg ',destination)
      end
   else
   	  begin
   	  		disks:=5
   	  end;
   Hanoi:=disks
   end;
       

begin
write('Enter the number of disks: ');
readln(disks);
write('Solution:');
disks:=Hanoi('A','B','C',disks)
end."
export output="SYNTAX OK.
"
export retCode=0