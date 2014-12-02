/**
 * \file SYN_parser.h
 * \brief Kniznica pracujuca so syntaxom
 * \author Palo Vican
 */ 
#ifndef h_SYN_parser
#define h_SYN_parser
#include<stdbool.h>
#include<stdio.h>
#include "LEX_lexem.h"

typedef enum {
    LOCAL,
    GLOBAL,
    PARAM,
    FUNCTION
} TTypeIdentEnum;

typedef enum {
    TERM,
    PLUS,
    MULT,
    LBRACK,
    RBRACK,
    REL_OP,
    TERMINATOR,
    COMMA,
    NONTERMINAL,
    ZARAZKA
} TStackEnum;

typedef struct SItem {
    TStructLex* data;
    TStackEnum type;
    struct SItem *next;
} TItem;

typedef struct {
    TItem* top;
} TSynStack;

typedef struct {
    TStackEnum *RSide;
    int length;
} TRule;

/** SYN_decPrem
 * \brief Pravidlo na deklaraciu premenu (2,3 pravidlo) 
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_decPrem(FILE *f,TTypeIdentEnum typ_ident);

/** SYN_prem
 * \brief Pravidlo na deklaraciu premenu (4 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_prem(FILE *f,TTypeIdentEnum typ_ident);

/** SYN_type
 * \brief Pravidlo na urcenie typu premennej (7,8,9,10 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_type(FILE *f,PTStructLex id,TTypeIdentEnum typ_id);

/** SYN_nextPrem
 * \brief Pravidlo na deklaraciu dalsej premennej (5,6 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_nextPrem(FILE *f,TTypeIdentEnum typ_ident);

/** SYN_decFunc
 * \brief Pravidlo na deklaraciu funkcie (11,12 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_decFunc(FILE *f);

/** SYN_paramList
 * \brief Pravidlo na zoznam parametrov (13,14 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_paramList(FILE *f);

/** SYN_param
 * \brief Pravidlo na deklaraciu parametru (15 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_param(FILE *f);

/** SYN_endParam
 * \brief Pravidlo na ukoncenie zoznamu parametrov (16,17 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_endParam(FILE *f);

/** SYN_onlyDecFunc
 * \brief Pravidlo na definiciu fukncie (18,19 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_onlyDecFunc(FILE *f,TTypeIdentEnum typ_ident);

/** SYN_funcBody
 * \brief Pravidlo na telo fukncie (20 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_funcBody(FILE *f);

/** SYN_program
 * \brief Pravidlo na rozlozenie na dalsie pravidla a hlavne telo (1 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_program(FILE *f);

/** SYN_statementList
 * \brief Pravidlo na rozlozenie prikazov (21,22 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_statementList(FILE *f);

/** SYN_comStatement
 * \brief Pravidlo na zlozeny prikaz (23 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_comStatement(FILE *f);

/** SYN_statement
 * \brief Pravidlo na jednotlive prikazy (24,28,29,30,31 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_statement(FILE *f);

/** SYN_ifStatement
 * \brief Pravidlo na if prikaz (24 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_ifStatement(FILE *f);

/** SYN_elseStatement
 * \brief Pravidlo na jednotlive prikazy (25 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_elseStatement(FILE *f);

/** SYN_semicolons
 * \brief Pravidlo na strednik (26,27 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_semicolons(FILE *f);

/** SYN_whileStatement
 * \brief Pravidlo na while cyklus (28 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_whileStatement(FILE *f);

/** SYN_assignStatement
 * \brief Pravidlo na priradovaci prikaz (29 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_assignStatement(FILE *f);

/** SYN_readlnStatement
 * \brief Pravidlo na prikaz readln (30 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_readlnStatement(FILE *f);

/** SYN_writeStatement
 * \brief Pravidlo na prikaz write (31 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_writeStatement(FILE *f);

/** SYN_term
 * \brief Pravidlo na term (32,33,34,35 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_term(FILE *f);

/** SYN_nextTerm
 * \brief Pravidlo na dalsi term (36,37 pravidlo)
 * \param f File handle
 * \return TRUE ak je syntakticky spravna ak nie tak FALSE
 */
bool SYN_nextTerm(FILE *f);

/** SYN_parser
 * \brief Fukcia na spustenie syntaktickej analyzy
 * \param f File handle
 */
void SYN_parser(FILE *f);

/** SYN_readLexem
 * \brief `Fukcia na alokovanie a nacitanie lexemy z lexykalnej analyzy
 * \param f File handle
 */
void SYN_readLexem(FILE *f);

#endif
