{================================================}
{          PROGRAM NUMBER_OPERATIONS             }
{   <MADE BY Lukas Slouka & Miroslav Cibulka>    }
{                LANG: IFJ-14                    }
{================================================}

var iterator    : integer;
    startNum    : integer;
    endNum      : integer;
    converted   : real;
    odmocnina   : real;
    parne       : boolean;
    mocnina     : integer;
    mocninaThree: integer;
    mocninaFour : integer;
    absHodnota  : integer;
    factorial   : integer;
    arcsinus    : real;

function convert(num:integer)             :real;    forward;
function powerInt(num:integer; ex:integer):integer; forward;  
function sqrt(N: real)                    :real;    forward;
function jeParne(num:integer)             :boolean; forward;    
function absInt(num:integer)              :integer; forward;
function __abs(x: real; y: real)          :real;    forward;
function fact(n: integer)                 :integer; forward;
function abs(x: real)                     :real;    forward;
function asin(z: real)                    :real;    forward;

function convert(num:integer):real;
{funkcia, ktora skonvertuje integer na real velmi neuspornym sposobom}
var result:real;
begin
    result := 0.0;
    if num >= 0 then begin
        while num > 0 do begin
            num := num - 1;
            result := result + 1.0
        end
    end else begin
        while num < 0 do begin
            num := num + 1;
            result := result - 1.0
        end
    end;
    convert := result
end;


function powerInt(num:integer; ex:integer):integer;
var pom:integer;
    result:integer;
begin
    pom := 1;
    result := num;
    if ex = 0 then begin
        result := 1
    end else begin
        while(pom < ex) do begin
            result := result * num;
            pom := pom+1
        end
    end;
    powerInt := result
end;


function absInt(num:integer):integer;
var pom : integer;
begin
    if num>0 then begin
        pom:=num
    end else begin
        pom := num - 2*num
    end;
    absInt := pom
end;

function __abs(x: real; y: real): real;
var pom: real;
begin
    if x>y then begin
        pom:=x-y
    end else begin
        pom:=y-x
    end;
    __abs:=pom
end;
    
function sqrt(N: real): real;
var newApproximation:real;
    oldApproximation:real;
    pom:real;
begin
    if (N <= 0.0) then begin
        sqrt:=0.0
    end else begin
        pom := N;
        oldApproximation := 1.0;
        while pom > 0.0000000001 do begin
            newApproximation :=  (oldApproximation + N/oldApproximation)/2.0;
            pom := __abs(newApproximation, oldApproximation);
            oldApproximation := newApproximation
        end;
        sqrt:=newApproximation
    end
end;


function jeParne(num:integer):boolean;
{hrozne neuspornym sposobom zisti ci je cislo parne}
var pom : integer;
    result : integer;
    dalsieCislo : integer;
begin
    pom := absInt(num);
    dalsieCislo := 0;
    if pom = 0 then begin
        jeParne := true
    end else begin
        while dalsieCislo < num do begin
            dalsieCislo := dalsieCislo + 2
        end;
    
        result := dalsieCislo / pom;
        if result = 0 then begin
            jeParne := true
        end else begin
            jeParne := false
        end
    end
end;

function fact(n: integer): integer;
var pom: integer;
begin
    if n <= 0 then begin
        fact := 1
    end else begin
        pom:=n-1;
        pom:=fact(pom);
        fact:=pom*n
    end
end;

function abs(x: real): real;
begin
    if x<0.0 then begin
        abs:=0.0-x
    end else begin
        abs:=x
    end
end;

function asin(z: real): real;
var yNext:  real;
    result: real;
    eps:    real;
    a:      real;
    poc:    real;
    pom:    real;
    PI:     real;
begin
    PI:=3.141592653589793238462643383279;
    yNext:=z;
    result:=z;
    eps:=0.1e-10;
    a:=0.0-1.0;
    poc:=0.0;
    pom:=abs(z);
    if pom > 0.71 then begin
        pom:=1.0-z*z;
        pom:=sqrt(pom);
        pom:=asin(pom);
        asin:=(PI/2.0)-pom
    end else begin
        pom:=1.0;
        while pom >= eps do begin
            pom:=abs(yNext);
            poc:=poc+1.0;
            a:=a+2.0;
            yNext:=(yNext*z*z*a*a)/((a+1.0)*(a+2.0));
            result:=result+yNext
        end;
        asin:=result
    end
end;

function below1(z: real): real;
var pom: real;
begin
    if z < 0.0 then begin
        pom:=0.0-1.0;
        while z < pom do begin
            z:=z/10.0
        end
    end else begin
        while z > 1.0 do begin
            z:=z/10.0
        end
    end;
    below1:=z
end;

function dohanoi(N:integer; Tfrom:integer; Tto:integer; Tusing:integer):integer;
VAR pom:integer;
  BEGIN
    if N > 0 THEN
    BEGIN
        pom:=N-1;
        pom:=dohanoi(pom, Tfrom, Tusing, Tto);
        pom:=N-1;
        pom:=dohanoi(pom, Tusing, Tto, Tfrom)
    END ELSE BEGIN END;
    dohanoi:=1
END;


begin
    startNum := 0-10;
    endNum := 10;
    iterator := startNum;
    while iterator <= endNum do begin
        mocnina := powerInt(iterator, 2);
        mocninaThree := powerInt(iterator, 3);
        mocninaFour := powerInt(iterator, 4);
        factorial := fact(iterator);
        parne := jeParne(iterator);
        absHodnota := absInt(iterator);
        if parne = true then begin
        end else begin end;
        converted := convert(iterator);
        if iterator < 1 then begin
        end else begin
            odmocnina := sqrt(converted)
        end;
        converted := below1(converted);
        arcsinus := asin(converted);
        iterator := iterator + 1
    end;
    iterator:=dohanoi(10, 1, 3, 2)
end.
