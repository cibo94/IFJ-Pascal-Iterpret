/**
 * \file error.h                       
 * \copyright 69DeamTeam
 * \author Miroslav Cibulka             
 * \brief          
 * Modul, ktory sluzi na:               
 * Logovanie, Error, Warningy<BR>
 * pri testovani treba pouzit prepinac pri preklade -DDEBUG
 */

#ifndef h_ERROR
#define h_ERROR
#include "LEX_lexem.h"
#include "INT_interpret.h"
/**
 * \brief Error function
 * \param retCode navratova hodnota
 * \param format text
 * \param ... arguments
 * \returns nothing
 * \see warning
 * \see log
 */
static inline void error (const int retCode, const char *format, ...) 
    __attribute__ ((unused, format(printf, 2, 3)));
/**
 * \brief Warning function
 * \param format text
 * \param ... arguments
 * \returns nothing
 * \see error
 * \see log
 */
static inline void warning (const char *format, ...) 
    __attribute__ ((unused, format(printf, 1, 2)));
/**
 * \brief Funkcia vypise dekodovane instrukcie
 * \param __EIP ukazatel na EIP
 * \returns NOTHING
 * \see P3AC
 * \see INT_interpret(...)
 */
static inline void print_EIP (P3AC *__EIP)
    __attribute__ ((unused));
/**
 * \brief Logging function -> Do not use this function use log() instead
 * \param line Argument for define log(...)
 * \param function Argument for define log(...)
 * \param file Argument for define log(...)
 * \param format text
 * \param ... arguments
 * \returns nothing
 * \see error
 * \see warning
 * \see log(...)
 */
static inline void __log(const int line, const char *function, const char *file, const char *format, ...) 
    __attribute__ ((unused, format(printf, 4, 5))); 

#define log(...) __log(__LINE__, __FUNCTION__, __FILE__, ##__VA_ARGS__)
#define ERR_LEX             1   //!< Lexikologicka chyba \see error
#define ERR_SYN             2   //!< Syntakticka chyba \see error
#define ERR_SEM_UNDEF       3   //!< Semanticka chyba - nedefinovana premenna \see error
#define ERR_SEM_TYPE        4   //!< Semanticka chyba - typova chyba \see error
#define ERR_SEM_OTHERS      5   //!< Semanticka chyba - ina \see error
#define ERR_RNTM_NUMREAD    6   //!< Behova chyba - citanie cisla \see error
#define ERR_RNTM_UNDEF      7   //!< Behoba chyba - nedefinovana premenna \see error
#define ERR_RNTM_ZERO       8   //!< Behova chyba - delenie nulou \see error
#define ERR_RNTM_OTHERS     9   //!< Behova chyba - ine \see error
#define ERR_INTERNAL       99   //!< Vnutorna chyba programu (napr. malloc vratil null a pod) \see error
#ifndef __GNUC__
#   define __attribute__(x)     //!< NOTHING
#endif
#define TIME_SIZE 9
#define FORMAT_TIME \
    do { \
        time_t rawtime; \
        time (&rawtime); \
        strftime (cas, TIME_SIZE, "%T", localtime (&rawtime));\
    } while (0);

static char cas[TIME_SIZE];
extern unsigned int LINE_NUM;
extern char *FILE_NAME;

__attribute__((unused))
static const char *ERROR_MSG[] = {
    "Normal exit", 
    "Lexikalna chyba", 
    "Syntakticka chyba", 
    "Semanticka chyba - nedefinovana premenna", 
    "Semanticka chyba - typova",
    "Semanticka chyba - ostatne", 
    "Behova chyba - nacitanie ciselnej hodnoty zo vstupu",
    "Behova chyba - praca s neinicializovanou premennou",
    "Behova chyba - delenie nulou",
    "Behova chyba - ostatne",
    "Interna chyba programu"
};

__attribute__((unused))
static const char *OPERATIONS[] = {
    "PLUS", 
    "MINUS", 
    "MUL", 
    "DIV", 
    "ASSIGN", 
    "LESS",
    "GREAT",
    "LESSEQ",
    "GREATEQ", 
    "EQUAL",
    "NEQUAL",
    "CALL", 
    "RET", 
    "PUSH", 
    "POP", 
    "JTRUE", 
    "JMP", 
    "NOP", 
    "LOAD",
    "NOT",
    "STORE",
    "EBPPUSH",
    "EBPPOP"
};

__attribute__ ((unused))
static const char *TERM_TYPE[] = {
    "TERM_INT",
    "TERM_REAL",
    "TERM_STRING",
    "TERM_BOOL",
    "TERM_EIP",
    "TERM_POINTER",
    "TERM_OFFSET",
    "TERM_EMB"
};

extern char *KEY_WORDS[]; 

static inline void
error(const int retCode,
      const char *format,
      ...) {
     FORMAT_TIME;
     va_list args;
     va_start(args, format);
     fprintf (stderr,
#ifdef h_LEX_lexem
              "[%s] Line %d: File <%s>: ERROR: %s\n",
              cas,
              LINE_NUM,
              FILE_NAME != NULL ? FILE_NAME : "UNKNOWN",
#else
              "[%s] Line ???: File <%s>: ERROR: %s\n",
              cas,
              FILE_NAME != NULL ? FILE_NAME : "UNKNOWN",
#endif /*defined(h_INIT)*/
              ERROR_MSG[retCode == ERR_INTERNAL ? 10 : retCode]);
     vfprintf(stderr,
              format,
              args);
     fprintf (stderr,
              "\nExitCode %d\n",
              retCode);
     va_end(args);
     exit(retCode);
}

static inline void
warning(const char *format,
        ...) {
      FORMAT_TIME;
      va_list args;
      va_start(args, format);
      fprintf (stderr,
#ifdef h_LEX_lexem
              "[%s] Line %d: File <%s>: WARNING:\n",
              cas,
              LINE_NUM,
#else
              "[%s] Line ???: File <%s>: WARNING:\n",
              cas,
#endif /*defined(h_INIT)*/
              FILE_NAME != NULL ? FILE_NAME : "UNKNOWN"
              );
      vfprintf(stderr,
               format,
               args);
      fprintf (stderr, "\n");
      va_end(args);
}

/**
 * Ak je definovany DEBUG tak logovacie vystupy pojdu na stdout
 */
#ifndef DEBUG

static inline void
__log(const int   line,
      const char *function,
      const char *file,
      const char *format,
      ...) {
    FILE * f = fopen("log", "a");
    va_list args;
    va_start(args, format);
    if (f) {
        FORMAT_TIME;
        fprintf (f,
                 "[%s] LOG: Line %d: File %s: Function %s():\n\t",
                 cas,
                 line,
                 file,
                 function);
        vfprintf(f,
                 format,
                 args);
        fprintf (f, "\n");
    }
    va_end(args);
    fclose(f);
}

/**
 * Ak nie tak vystupy pojdu do logovacieho suboru "log"
 */
#else /*not defined(DEBUG)*/
static inline void
__log(const int   line,
      const char *function,
      const char *file,
      const char *format,
      ...) {
    FORMAT_TIME;
    va_list args;
    va_start(args, format);
    fprintf (stdout,
            "[%s] LOG: Line %d: File %s: Function %s():\n\t",
            cas,
            line,
            file,
            function);
    vfprintf(stdout,
             format,
             args);
    fprintf (stdout, "\n");
    va_end(args);
}

#endif /*not defined(DEBUG)*/
#ifdef h_INT_PAR
static inline void print_EIP (P3AC *__EIP) {
    for (P3AC *POS=__EIP;*__EIP!=NULL;__EIP++) {
        printf("#%08u:\t%s\t%s%s%s%s%s\t{%d, %d, %d}\n",
          (unsigned int)(__EIP-POS+1),
          OPERATIONS[(*__EIP)->op],
          (*__EIP)->op1 != NULL ? (*__EIP)->op1->name            : "",
          (*__EIP)->op2 != NULL &&(*__EIP)->op1 != NULL ?  ", "  : "",
          (*__EIP)->op2 != NULL ? (*__EIP)->op2->name            : "", 
          (*__EIP)->ret != NULL &&((*__EIP)->op2 != NULL || (*__EIP)->op1 != NULL) ?  ", "   : "",
          (*__EIP)->ret != NULL ? (*__EIP)->ret->name            : "",
          (*__EIP)->op1 != NULL ? (*__EIP)->op1->value.integer   : 0, 
          (*__EIP)->op2 != NULL ? (*__EIP)->op2->value.integer   : 0,
          (*__EIP)->ret != NULL ? (*__EIP)->ret->value.integer   : 0);
    }
}
#else  /*if defined(h_INT_PAR)*/

#endif /*if defined(h_INT_PAR)*/
#endif /*defined(h_ERROR)*/
