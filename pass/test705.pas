{ Test prirazeni a opetovneho cteni z navratove hodnoty funkce  - free pascal to bere }
var
	a: integer;

function f(): integer;
var
	x: integer;
begin
	f:=10;
	x:=f;
	f:=x
end;

begin
	a:=f();
	write(a)
end.