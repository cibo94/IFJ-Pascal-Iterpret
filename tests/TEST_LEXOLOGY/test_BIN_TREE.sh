#!/usr/bin/env bash
export dir="BIN_TREE"
export name="BIN_TREE"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"
int main() {
    
    TSbinstrom pom,pom2,pom3;

    

    PTStructLex lex = malloc (sizeof (TStructLex));
    lex->lex = "glob5";
    pom=BS_Add(NULL,lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob4";
    BS_Add(pom, lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob8";
    BS_Add(pom,lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob9";
    BS_Add(pom,lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob1";
    BS_Add(pom,lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob7";
    pom3=BS_Add(pom,lex);

    lex= malloc (sizeof (TStructLex));
    lex->lex = "glob2";
    BS_Add(pom,lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob3";
    BS_Add(pom,lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob0";
    BS_Add(pom,lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob6";
    BS_Add(pom,lex);

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob10";
    BS_Add(pom,lex);

    BS_Print(pom);

    

    PTStructLex lex2 = malloc (sizeof (TStructLex));
    lex2->lex = "var5";
    pom2 =BS_Add(NULL,lex2);

    lex2 = malloc (sizeof (TStructLex));
    lex2->lex = "bla4";
    BS_Add(pom2, lex2);

    lex2 = malloc (sizeof (TStructLex));
    lex2->lex = "kek";
    BS_Add(pom2,lex2);

    lex2 = malloc (sizeof (TStructLex));
    lex2->lex = "super dlhy retazec";
    BS_Add(pom2,lex2);

    lex2 = malloc (sizeof (TStructLex));
    lex2->lex = "boli ma hlava";
    BS_Add(pom2,lex2);

    lex2= malloc (sizeof (TStructLex));
    lex2->lex = "x";
    BS_Add(pom2,lex2);

    lex2= malloc (sizeof (TStructLex));
    lex2->lex = "pomuk";
    BS_Add(pom2,lex2);
    //BS_Add(pom2,lex2);

    pom3->loc_table=pom2;
   
    lex2 = malloc (sizeof (TStructLex));
    lex2->lex = "bla4";
    BS_Print(pom2);
    BS_Print(BS_Find(pom2,lex2));
    BS_Print(BS_Find(pom,lex2));

    BS_Free(pom);
    free(lex);
	return 0;
    
}'
export input=""
export output="
"
export retCode=0
