#!/bin/bash 
export dir="TYPE"
export name="TYPE"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"
int main() {
    TStructLex l;
    l.lex = "begin";
    isKeyWord(&l);
    printf("%s %s\n","begin", KEY_WORDS[l.type]);
	return 0;
}'
export input=""
export output="begin begin
"
export retCode=0
