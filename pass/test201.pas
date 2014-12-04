{------------------------- Syntakticke testy ----------------------------------}
{------------------- Detekcia syntaktickych chyb ------------------------------}
{ Testovanie na odhalenie nespravnej postupnosti sekcii programu							 }
{ Definicia uzivatelskych funkcii vymenena s deklaraciou glob. premennych      }

function my_func() : boolean;
var local_var : boolean;
begin
end;

var global_var : boolean;

begin
end.
