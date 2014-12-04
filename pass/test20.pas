{ Test rekurze }

var
	num: integer;
	tmp: integer;


function print(cislo: integer): integer;
var
	tmp: integer;
begin
	write(cislo, ''#10'');

	if 0 < cislo then
	begin
		print := print(cislo-1)
	end
	else
	begin
		print := 0
	end;

	write(cislo, ''#10'')
end;


{ Hlavni program }
begin
	write('Zadejte kolikrat se ma rekurzivne volat: ');
	readln(num);
	tmp:=print(num)
end.
