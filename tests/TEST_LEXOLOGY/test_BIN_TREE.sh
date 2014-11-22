#!/usr/bin/env bash
export dir="BIN_TREE"
export name="BIN_TREE"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"
int main() {
    
    TSbinstrom pom;
    pom=BS_New(NULL,5);
    BS_Add(pom, NULL, 4);
    BS_Add(pom, NULL, 8);
    BS_Add(pom, NULL, 9);
    BS_Add(pom, NULL, 1);
    BS_Add(pom, NULL, 7);
    BS_Add(pom, NULL, 2);
    BS_Add(pom, NULL, 3);
    BS_Add(pom, NULL, 0);
    BS_Add(pom, NULL, 6);
    BS_Add(pom, NULL, 10);

    BS_Print(pom);
    BS_Free(pom);

	return 0;
    
}'
export input=""
export output="0
3
2
1
4
6
7
10
9
8
5
"
export retCode=0
