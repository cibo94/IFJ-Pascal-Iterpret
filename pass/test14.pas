{ Hledani korenu rovnice pomoci secen }

var
	x1: real;
	x2: real;
	tmp1: real;
	tmp2: real;

function r_abs(x: real): real;
begin
	if x < 0.0 then
	begin
		r_abs := x * (0-1)
	end
	else
	begin
		r_abs := x
	end
end;

{ Test sloziteho vyrazu }
begin
	x1 := 1.0;
	x2 := 1.0;
	tmp1 := 1.0;
	tmp2 := 1.0;

	write(x1 * (x2 - (x1 * tmp1 * (x1/tmp1 + (x2 - (tmp2 + x1))))), ''#10'')
end.