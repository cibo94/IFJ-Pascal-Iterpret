#!/usr/bin/env bash
export dir="test9"
export name="test9"
export file="../src/LEX_lexem.c ../src/SYN_parser.c"
export src='#include "../src/../src/inc.h"

int main () {
    SYN_parser(stdin);
    print_EIP(EIP);
    return 0;
}'
export input="
{Boolean test - hlavne precedencni analyza...}

var 
  a : boolean;
  b : boolean;
  c : boolean;

begin
  a := true;
  b := false;

  write('a - true: ', a, ''#10'');
  write('b - false: ', b, ''#10'');

  c := a < b;
  write('a < b:  ', c, ''#10'');
  c := a > b;
  write('a > b:  ', c, ''#10'');
  c := a <= b;
  write('a <= b: ', c, ''#10'');
  c := a >= b;
  write('a >= b: ', c, ''#10'');
  c := a = b;
  write('a = b:  ', c, ''#10'');
  c := a <> b;
  write('a <> b: ', c, ''#10'');
  c := a > b;
  write('a > b:  ', c, ''#10'');

  if true then begin
    write('Bool v ifu...'#10'')
  end else begin
  end
end.
"
export output="a - true: TRUE
b - false: FALSE
a < b:  FALSE
a > b:  TRUE
a <= b: FALSE
a >= b: TRUE
a = b:  FALSE
a <> b: TRUE
a > b:  TRUE
Bool v ifu..."
export retCode=0
