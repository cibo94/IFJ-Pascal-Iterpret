{ Test interpretu - behova chyba neinicializovana promenna (navratovy kod 7) }

var
	tmp: integer;


function print(cislo: integer): integer;
var
	tmp: integer;
	uninit: integer;
begin
	write(cislo, ''#10'');

	if 0 < cislo then
	begin
		print := print(cislo-1)
	end
	else
	begin
		print := uninit
	end;

	write(cislo, ''#10'')
end;


{ Hlavni program }
begin
	tmp:=print(1000)
end.