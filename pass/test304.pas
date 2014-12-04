{------------------- Detekcia semantickych chyb ------------------------------}
{-------------------------- EXIT_CODE 3 --------------------------------------}
{ Dopredna deklaracia funkcie sa nachadza za definiciou                       }

function func(param:integer) : integer;
begin
end;

function func(param:integer) : integer; forward;

begin
end.
