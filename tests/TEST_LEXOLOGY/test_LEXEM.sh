#!/usr/bin/env bash
export dir="LEXEM"
export name="LEXEM"
export file="../src/LEX_lexem.c"
export src='#include "../src/inc.h"
int main(int argc, char **argv) {
    if (!argv[argc-1]) return 1;
    FILE *f = init(argv[argc-1]);
    TStructLex *ret = malloc(sizeof(TStructLex));
    if (!ret) return 1;
    ret->lex = NULL;
    ret->type = 0;
    while(LEX_getLexem(ret, f) != true) {
        printf("%s\n", ret->lex);
    }
    printf("%s\n", ret->lex);
    free(ret);
    fclose(f);
    return 0;
}'
export input="sep sep;sep,sep.sep
sep-sep_sep=sep+sep-sep/sep*sep(sep)sep<sep>sep{dude}sep:sep10"
export output="sep
sep
;
sep
,
sep
.
sep
sep
-
sep_sep
=
sep
+
sep
-
sep
/
sep
*
sep
(
sep
)
sep
<
sep
>
sep
sep
:
sep10
"
export retCode=0