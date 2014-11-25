#!/usr/bin/env bash
export dir="BT"
export name="BT"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"

int main (int argc, char ** argv) {
    FILE *f = init (argv[argc-1]);
    TSbinstrom str = BS_New (NULL, hash("", 0, ""));

    /* MAIN */

    PTStructLex lex = malloc (sizeof (TStructLex));
    lex->lex = "glob1";
    BS_Add(str, (void *)lex, hash("", 0, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "glob2";
    BS_Add(str, (void *)lex, hash("", 0, lex->lex));

    /* F_a */

    lex = malloc (sizeof (TStructLex));
    lex->lex = "F_a";
    BS_Add(str, (void *)lex, hash(lex->lex, 0, ""));
    
    lex = malloc (sizeof (TStructLex));
    lex->lex = "gsdf";
    BS_Add(str, (void *)lex, hash("F_a", 1, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "sdfg";
    BS_Add(str, (void *)lex, hash("F_a", 2, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "twret";
    BS_Add(str, (void *)lex, hash("F_a", 3, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "var1";
    BS_Add(str, (void *)lex, hash("F_a", 0, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "var2";
    BS_Add(str, (void *)lex, hash("F_a", 0, lex->lex));

    /* F_b */

    lex = malloc (sizeof (TStructLex));
    lex->lex = "F_b";
    BS_Add(str, (void *)lex, hash(lex->lex, 0, ""));
    
    lex = malloc (sizeof (TStructLex));
    lex->lex = "param1";
    BS_Add(str, (void *)lex, hash("F_b", 1, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "var1";
    BS_Add(str, (void *)lex, hash("F_b", 0, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "var2";
    BS_Add(str, (void *)lex, hash("F_b", 0, lex->lex));

    /* abc */

    lex = malloc (sizeof (TStructLex));
    lex->lex = "abc";
    BS_Add(str, (void *)lex, hash(lex->lex, 0, ""));
    
    lex = malloc (sizeof (TStructLex));
    lex->lex = "param1";
    BS_Add(str, (void *)lex, hash("abc", 1, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "var1";
    BS_Add(str, (void *)lex, hash("abc", 0, lex->lex));

    lex = malloc (sizeof (TStructLex));
    lex->lex = "var2";
    BS_Add(str, (void *)lex, hash("abc", 0, lex->lex));

    BS_Print(str);
    printf("\n\nHladanie podstromu (F_b, 0, var1)");
    BS_Print(BS_Find(str, hash("F_b", 0, "var1")));
    

    printf("\n\nHladanie podstromu (F_a, 1, param1)");
    BS_Print(BS_Find(str, hash("F_b", 0, "var1")));

    BS_Free(str);
    fclose(f);
    return 0;    
}'
export input="
"
export output=""
export retCode=0
