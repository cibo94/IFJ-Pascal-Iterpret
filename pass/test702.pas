{ Test interpretu - behova chyba neinicializovana promenna (navratovy kod 7) }
var
	x:integer;

function f():integer;
var
	a:integer;
begin
  	f:=3*4*8+1345 + a
end;

begin
	x:=f()
end.