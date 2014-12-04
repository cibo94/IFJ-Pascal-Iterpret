{------------------- Detekcia semantickych chyb ------------------------------}
{-------------------------- EXIT_CODE 3 --------------------------------------}
{ Dopredna deklaracia funkcie ma odlisnu hlavicku ako definicia               }

function func(diff_param:integer) : string; forward;

function func(param:integer) : string;
begin
end;


begin
end.
