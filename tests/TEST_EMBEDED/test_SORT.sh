#!/usr/bin/env bash
export dir="SORT"
export name="SORT"
export file="../src/emb_func.c ../src/LEX_lexem.c"
export src='#include "../src/inc.h"
int main(int argc, char**argv) {
    FILE*f=fopen(argv[argc-1], "r");
    char *tmp = malloc(11);
    char *s = EMB_sort(fgets(tmp,11,f), 10);
    printf("%s", s);
	free(s);
	fclose(f);
	return 0;
}'
export input="7894561230"
export output="0123456789"
export retCode=0