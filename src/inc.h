/** Includes
 * \file inc.h
 * \author 69DreamTeam
 * \brief All includes goes here!
 */

/** Standart includes		*/
#include <stdlib.h>			//!< malloc, etc.
#include <stdio.h>			//!< Standartna IO kniznica
#include <ctype.h>     			//!< PRIDANA KNIZNICA (funkcia isdigit)
#include <time.h>			//!< Praca s casom pouzite v kniznici 'error.h'
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

/** Our includes 		*/
#include "error.h"			    //!< Error handling
#include "LEX_lexem.h"			//!< Praca s lexemami
#include "binary_tree.h"		//!< Binarny strom
#include "emb_func.h"
#include "INT_interpret.h"
#include "init.h"               //!< Inicializacne funkcie
#include "INT_parser.h"
