{ Test interpretu - behova chyba neinicializovana promenna (navratovy kod 7) }
var
	x:integer;

function f():integer;
var
	a:integer;
begin
  	a := 1 * 2 * 3 * 4
end;

begin
	x:=f()
end.