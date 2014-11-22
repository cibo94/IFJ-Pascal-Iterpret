/** Inicialization
 * \file init.h
 * \author 69DreamTeam
 * \brief All inicialization functions goes here!
 */

#ifndef h_INIT
#define h_INIT

#define constructor __attribute__((unused, constructor))
#define destructor __attribute__((unused, destructor))

typedef struct {
    TSbinstrom SYM_TABLE;
} GLOB_DEST, *PGLOB_DEST;

extern char *FILE_NAME;

FILE *init(const char *fn);
void  interpret();

#endif
