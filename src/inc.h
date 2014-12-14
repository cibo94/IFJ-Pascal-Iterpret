/** Includes
 * \file inc.h
 * \author Miroslav Cibulka - xcibul10
 * \brief Subor v ktorom su vsetky includy
 */

/** Standart includes		*/
#include <stdlib.h>             //!< malloc, etc.
#include <stdio.h>              //!< Standartna IO kniznica
#include <ctype.h>              //!< PRIDANA KNIZNICA (funkcia isdigit)
#include <time.h>			    //!< Praca s casom pouzite v kniznici 'error.h'
#include <string.h>             //!< Praca so stringami
#include <stdarg.h>             //!< Praca s arg
#include <stdbool.h>            //!< boolean
#include <stdint.h>             //!< velkosti intov

/** Our includes 		*/
#include "error.h"              //!< Error handling
#include "lex_lexem.h"          //!< Praca s lexemami
#include "ial.h"    	        //!< Binarny strom
#include "syn_parser.h"         //!< Syntakticky parser
#include "int_interpret.h"      //!< interpret
#include "init.h"               //!< Inicializacne funkcie
#include "sem.h"                //!< semantika
