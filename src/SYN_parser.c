#include "inc.h"


static PTStructLex lexema;

const char* prec_table[] = {
"0>>=>>>>", /*term*/
"<><<>>>0", /*plus*/
"<>><>>>0", /*krat*/
"<<<<=<0<", /*l zatvorka*/
"0>>0>>>0", /*p zatvorka*/
"<<<<>>>0", /*rel. operatory*/
"<<<<0<00", /*ukoncujuci znak = dno zasobniku*/
"<000>00>"  /*ciarka*/
}; 
static TStackEnum
Rule1[] = {TERM},
Rule2[] = {NONTERMINAL,PLUS,NONTERMINAL},
Rule3[] = {NONTERMINAL,MULT,NONTERMINAL},
Rule4[] = {RBRACK,NONTERMINAL,LBRACK},
Rule5[] = {NONTERMINAL,REL_OP,NONTERMINAL},
Rule6[] = {RBRACK,NONTERMINAL,LBRACK,TERM},
Rule7[] = {NONTERMINAL,COMMA,NONTERMINAL};

void prevodnik(TEnumLexem in, TStackEnum *out);

void StackInit(TSynStack *S) {
    TItem *item = malloc(sizeof(TItem));
    item->data = NULL;
    item->next = NULL;
    item->type = TERMINATOR;
    S->top = item;
}

int SEmpty(TSynStack *S) { /* toto puzivam len v SDispose*/
    return S->top->type == TERMINATOR;
}

TItem *STop(TSynStack *S) {
    return S->top;
}

void SPop(TSynStack *S) {
    if (!SEmpty(S)) {    
	TItem *pom = S->top;
	S->top = S->top->next;
	//free(pom->data);
	free(pom);
    }
}
void ChooseRule(TStackEnum top_terminal, int isfunc, int *relused,TRule *rule) {
    switch (top_terminal) {
	case TERM:
	    //printf("1\n");
	    rule->RSide = Rule1;
	    rule->length = 1;
	    break;
	case PLUS:
	    //printf("2\n");
	    rule->RSide = Rule2;
	    rule->length = 3;
	    break;
	case MULT:
	    //printf("3\n");
	    rule->RSide = Rule3;
	    rule->length = 3;
	    break;
	case RBRACK:
	    if (isfunc) {
		//printf("6\n");
		rule->RSide = Rule6;
		rule->length = 4;
	    }
	    else {
		//printf("4\n");
		rule->RSide = Rule4;
		rule->length = 3;
	    }
	    break;
	case REL_OP:
	    if (*relused) { rule->RSide = NULL; break; }
	    //printf("5\n");
	    rule->RSide = Rule5;
	    rule->length = 3;
	    *relused = 1;
	    break;
	case COMMA:
	    if (isfunc) {
		//printf("7\n");
		rule->RSide = Rule7;
		rule->length = 3;
	    }
	    else { rule->RSide = NULL;}
	    break;
	default: 
	    rule->RSide = NULL;
    }
    return;
}

void SPush(TSynStack *S, TStructLex* D) {
    TItem* item = malloc(sizeof(TItem));
    item->data = D;
    prevodnik(D->type, &item->type);
    item->next = S->top;
    S->top = item;
}
void SPush_nonterminal(TSynStack *S) {
    TItem *item = malloc(sizeof(TItem));
    item->data = NULL;
    item->type = NONTERMINAL;
    item->next = S->top;
    S->top = item;
}

void SPush_zarazka(TSynStack *S) {
    TItem *item = malloc(sizeof(TItem));
    item->data = NULL;
    item->type = ZARAZKA;
    item->next = S->top;
    S->top = item;
}

void SDispose(TSynStack *S) {
    while (!SEmpty(S)) SPop(S);
    //SPop(S);
}

void prevodnik(TEnumLexem in, TStackEnum *out) {
    if (in >= OPERATOR_GREATER && in <= OPERATOR_NEQUAL) {
	*out = REL_OP;
	return;
    }
    switch(in) {
     case KEY_TRUE:
	*out = TERM;
	break;
	 case KEY_FALSE:
	*out = TERM;
	break;
    case INT_CONST:
	*out = TERM;
	break;
    case REAL_CONST:
	*out = TERM;
	break;
    case STRING_CONST:
	*out = TERM;
	break;
    case IDENTIFICATOR:
	*out = TERM;
	break;
    case LBRACKET:
	*out = LBRACK;
	break;
    case RBRACKET: 
	*out = RBRACK;
	break;
    case OPERATOR_PLUS: 
	*out = PLUS;
	break;
    case OPERATOR_MINUS:
	*out = PLUS;
	break;
    case OPERATOR_TIMES:
	*out = MULT;
	break;
    case OPERATOR_DIV:
	*out = MULT;
	break;
    case CIARKA:
	*out = COMMA;
	break;
    default:
	*out = TERMINATOR;
    } /* switch */
} /*void prevodnik*/

bool SYN_expression(FILE *f,bool assigned) {
    TRule *rule;
    TSynStack stack1;
    int ind, /*index na prechadzanie pravidiel*/
	relused = 0, /*bool - indikuje pouzitie relacneho operatora - akceptujeme 1 vo vyraze*/
	isfunc = 0, /*bool na zistenie ci ide o volanie funkcie*/
	popped; /*bool - popoval sa neterminal a treba ho vratit?*/
	//assigned = 1; /*ci ide o priradenie alebo nie - natvrdo true ako si vravel*/
    TItem *pom;
	TStructLex *funid;
    StackInit(&stack1); 
    rule = malloc(sizeof(TRule)); /*premenna na vyber pravidla*/
    //pom = malloc(sizeof(TItem)); /* premenna pre hodnotu na vrchole zasobniku */

     
    TStackEnum input_lex, top_terminal = TERMINATOR; /*prazdny zasobnik - dno*/
    prevodnik(lexema->type, &input_lex);
    if (lexema->type == IDENTIFICATOR && assigned) { /*kontrola, ci sa jedna o volanie funkcie*/
		SPush_zarazka(&stack1);	/*jedine miesto, kde sa pracuje s premennou "assigned"  je tu*/
		SPush(&stack1, lexema);
		funid = lexema;
		top_terminal = TERM;
		SYN_readLexem(f);
		prevodnik(lexema->type, &input_lex);
		if (input_lex == LBRACK) {
			isfunc = 1; SEM_fCallPrologue(funid);
			SPush(&stack1, lexema);
			top_terminal = LBRACK;
			SYN_readLexem(f);
			prevodnik(lexema->type, &input_lex);
			if (input_lex == RBRACK) {/* skusa ci je bez parametrov OK*/
				SYN_readLexem(f);
				SEM_functionCall(funid);
				return true;
			}	
		}
    }
    if (top_terminal == input_lex && input_lex == TERMINATOR) return false;
	/* osetrenie prazdneho vyrazu */
    while (top_terminal != input_lex || input_lex != TERMINATOR) { /*top_terminal sa nerovna input_lex a to sa nerovna ukoncovaciemu symbolu*/
	//??
	pom = STop(&stack1);
	switch (prec_table[top_terminal][input_lex]) {
	    case '>':
		ChooseRule(top_terminal, isfunc, &relused,rule); /*vyberie a posle pravidlo; isfunc - globalne?*/
		if (rule->RSide == NULL) return false;
		ind = 0; 
		while (pom->type != ZARAZKA && ind < rule->length) { /*porovnava obsah zasobnika s*/
		    if (pom->type != rule->RSide[ind]) {		/* pravou stranou vybraneho pravidla*/
			
			return false; /*pri kazdom volani printf("syntax error") treba dealokaciu vsetkeho */
		    } else {
			    if (ind == 0 && rule->RSide[ind] == TERM) {
				if (isfunc) SEM_functionParam(funid, pom->data);
				else SEM_createLeaf(pom->data);
				}
				if (ind == 1 && rule->length == 3 && rule->RSide[1] != COMMA && rule->RSide[1] != NONTERMINAL) 
				  SEM_createTree(pom->data);
				if (ind == 3) 
				  SEM_functionCall(funid);
			}
		    SPop(&stack1);
		    pom = STop(&stack1); /*az kym nenarazi na zarazku alebo */
		    ind++; /*az kym neporovna s celou pravou stranou pravidla */
		}
		if (ind == rule->length && pom->type == ZARAZKA) {
		    SPop(&stack1);
		    if (rule->length == 4) return true; /*indikacia pouzitia 6.pravidla - funkcia, uspesny koniec*/
		    top_terminal = stack1.top->type;
		    SPush_nonterminal(&stack1);
		}
		else {
		    return false;
		}
		break;
	    case '<':
		popped = 0;
		if (pom->type == NONTERMINAL) { /* ak je na vrchole neterminal, vyberie ho - max jeden*/
		    SPop(&stack1);
		    popped = 1; /*true*/
		    pom = STop(&stack1);
		}
		if (pom->type != NONTERMINAL) SPush_zarazka(&stack1); /* vlozi zarazku */
		else {return false; }/*viac ako jeden NEterminal na vrchole-chyba*/
		if (popped) { SPush_nonterminal(&stack1);} /* vrati neterminal spat na zasobnik */
		SPush(&stack1, lexema); /* vlozi novy neterminal, vezme dalsiu lexemu */
		pom = STop(&stack1); 
		top_terminal = pom->type;
		SYN_readLexem(f);
		prevodnik(lexema->type, &input_lex);
		break;
	    case '=':
		SPush(&stack1, lexema);
		pom = STop(&stack1); 
		top_terminal = pom->type;
		SYN_readLexem(f);
		prevodnik(lexema->type, &input_lex);
		break;
	    case '0':
		return false;  /*doplnit free*/
	}
    } /*while ValidLex */
    /*pomvypis(&stack1);*/
    /*SDispose(&stack1);*/
    //printf("krasne\n"); krasne!!!

/* KONFLIKT s Palovou castou
 * palo este pouzival danu adresu

    free(pom);
    
*/
    free(rule);
    return true;
}

void SYN_readLexem(FILE *f){
  
  /*switch (lexema->type) {
    case IDENTIFICATOR:
    case INT_CONST:
    case REAL_CONST:
    case STRING_CONST:
    case KEY_TRUE:
    case KEY_FALSE:
      break;
    default:
      free(lexema->lex);
      free(lexema);
  }*/
  lexema=(PTStructLex)malloc(sizeof(TStructLex));
  if (lexema==NULL) error(ERR_INTERNAL, "Chyba alokacie pamete");
  LEX_getLexem(lexema,f); 
}

bool SYN_funcBody(FILE *f){

  if (!SYN_comStatement(f)) return false;
  if (lexema->type!=STREDNIK) return false;
  SEM_endFunctionDef(lexema);  // Semanticka akcia
  SYN_readLexem(f);
  return true;
}

bool SYN_onlyDecFunc(FILE *f,TTypeIdentEnum typ_ident){

  switch(lexema->type){
    case KEY_VAR:
    case KEY_BEGIN:
      if (SYN_decPrem(f,typ_ident) && SYN_funcBody(f) && SYN_decFunc(f)) return true;
      else return false;
    case KEY_FORWARD:
      SEM_endFunctionDef(lexema); // Semanticka akcia
      SYN_readLexem(f);
      if (lexema->type!=STREDNIK) return false;
      SYN_readLexem(f);
      if (SYN_decFunc(f)) return true;
      else return false;
    default:
      return false;
  }
}

bool SYN_endParam(FILE *f){

  switch (lexema->type){
    case STREDNIK:
      SYN_readLexem(f);
      if (SYN_param(f)) return true;
      else return false;
    case RBRACKET:
      return true;
    default:
      return false;
  }
}

bool SYN_param(FILE *f){
  PTStructLex temp_lex=lexema;
    
  if (lexema->type!=IDENTIFICATOR) return false;
  SYN_readLexem(f);
  if (lexema->type!=DDOT) return false;
  SYN_readLexem(f);
  if (!SYN_type(f,temp_lex,PARAM)) return false;
  if (SYN_endParam(f)) return true; 
  else return false;
}

bool SYN_paramList(FILE *f){

  switch(lexema->type){
    case IDENTIFICATOR:
	  if (SYN_param(f)) return true;
	  else return false;
	case RBRACKET:
	  return true;
	default:
	  return false;	
  }
}

bool SYN_decFunc(FILE *f){
  PTStructLex temp_lex;

  switch (lexema->type){
  	case KEY_FUNCTION:
  	  SYN_readLexem(f);
  	  if (lexema->type!=IDENTIFICATOR) return false;
  	  temp_lex=lexema;
          SEM_defineFunction(lexema); // Semanticka akcia
          SYN_readLexem(f);
  	  if (lexema->type!=LBRACKET) return false;
  	  SYN_readLexem(f);
  	  if (!SYN_paramList(f)) return false;
  	  if (lexema->type!=RBRACKET) return false;
  	  SYN_readLexem(f);
  	  if (lexema->type!=DDOT) return false;
  	  SYN_readLexem(f);
  	  if (!SYN_type(f,temp_lex,FUNCTION)) return false;	
  	  if (lexema->type!=STREDNIK) return false;
  	  SYN_readLexem(f);
  	  if (SYN_onlyDecFunc(f,LOCAL)) return true;
  	  else return false;
    case KEY_BEGIN:
      return true;
    default:
      return false;
  }
}

bool SYN_nextPrem(FILE *f,TTypeIdentEnum typ_ident){
  
  switch (lexema->type){
    case KEY_BEGIN:
    case KEY_FUNCTION:
    	return true;
    case IDENTIFICATOR:
    	if (SYN_prem(f,typ_ident) && SYN_nextPrem(f,typ_ident)) return true;
    	else return false;
    default:
    	return false;
  }
}

bool SYN_type(FILE *f,PTStructLex id,TTypeIdentEnum typ_id){
  
  switch (lexema->type){
    case KEY_INTEGER:
    case KEY_REAL:
    case KEY_STRING:
    case KEY_BOOLEAN:
      if (typ_id==GLOBAL) SEM_defineGlobal(id,lexema);      //Semanticka akcia
      else if (typ_id==LOCAL) SEM_defineLocal(id,lexema);   //Semanticka akcia
      else if (typ_id==PARAM) SEM_defineParam(id,lexema);   //Semanticka akcia
      else SEM_defFuntionType(lexema);                      //Semanticka akcia
      break;
    default:
      return false;
  }
  SYN_readLexem(f);
  return true;
}

bool SYN_prem(FILE *f,TTypeIdentEnum typ_id){
  PTStructLex temp_lex;

  if (lexema->type!=IDENTIFICATOR) return false;
  temp_lex=lexema;
  SYN_readLexem(f);
  if (lexema->type!=DDOT) return false;
  SYN_readLexem(f);
  if (!SYN_type(f,temp_lex,typ_id)) return false;
  if (lexema->type!=STREDNIK) return false;
  SYN_readLexem(f);
  return true;
}

bool SYN_decPrem(FILE *f,TTypeIdentEnum typ_ident){
  
  switch (lexema->type){
    case KEY_BEGIN:
    case KEY_FUNCTION:
	  return true;
	case KEY_VAR:
	  SYN_readLexem(f);
	  if (SYN_prem(f,typ_ident) && SYN_nextPrem(f,typ_ident)) return true;
	  else return false;
	default: 
	  return false;
  }
}
bool SYN_nextTerm(FILE *f){
  
  switch (lexema->type){
  	case RBRACKET:
  	  return true;
  	case CIARKA:
	  SYN_readLexem(f);
	  if (SYN_term(f) && SYN_nextTerm(f)) return true;
	  else return false;
	default:
	  return false;  
  }
}

bool SYN_term(FILE *f){
  
  switch (lexema->type){
    case IDENTIFICATOR:
	case INT_CONST:
	case REAL_CONST:
	case STRING_CONST:
	  SYN_readLexem(f);
          return true;
	default:
	  return false;
  }
}

bool SYN_writeStatemnet(FILE *f){
  
  if (lexema->type!=LBRACKET) return false;
  SYN_readLexem(f);
  if (SYN_term(f) && SYN_nextTerm(f)) {
  	if (lexema->type!=RBRACKET) return false;
  	SYN_readLexem(f);
  	return true;
  }
  else return false;
}

bool SYN_readlnStatemnet(FILE *f){
  
  if (lexema->type!=LBRACKET) return false;
  SYN_readLexem(f);
  if (lexema->type!=IDENTIFICATOR) return false;
  SYN_readLexem(f);
  if (lexema->type!=RBRACKET) return false;
  SYN_readLexem(f);
  return true;
}
bool SYN_assignStatemnet(FILE *f){
  
  if (lexema->type!=OPERATOR_ASSIGN) return false;
  SYN_readLexem(f);
  //SEM_createLeaf(lexema);                               // tu bola zmena
  // SKUSKA KVOLI  TESTOVANIU
  if (SYN_expression(f,true)) 
  return true;
  else 
  return false;
  //SYN_readLexem(f);
  return true;
}

bool SYN_whileStatemnet(FILE *f){
  
  if (!SYN_expression(f,false)) return false;
  if (lexema->type!=KEY_DO) return false;
  SEM_whileBegin();
  SYN_readLexem(f);
  if (SYN_comStatement(f)) return true;
  else return false;
}

bool SYN_elseStatemnet(FILE *f){
  
  if (lexema->type!=KEY_ELSE) return false;
  SYN_readLexem(f);
  SEM_elseStat();
  if (SYN_comStatement(f)) return true;
  else return false;
}

bool SYN_ifStatemnet(FILE *f){
  
  if (!SYN_expression(f,false)) return false;
  if (lexema->type!=KEY_THEN) return false;
  SYN_readLexem(f);
  SEM_thenStat();
  if (SYN_comStatement(f) && SYN_elseStatemnet(f)) return true;
  else return false;
  
}

bool SYN_statement(FILE *f){
  PTStructLex term_lex = lexema;
  
  switch (lexema->type){
  	case KEY_IF:
  	  SYN_readLexem(f);
  	  if (SYN_ifStatemnet(f)) {
	    SEM_endIf();
	    return true;
	  } else return false;
  	case KEY_WHILE:
  	  SEM_whileStat();
  	  SYN_readLexem(f);
  	  if (SYN_whileStatemnet(f)) {
		SEM_whileEnd();  
		return true;
	  } else return false;
  	case IDENTIFICATOR:
  	  SYN_readLexem(f);
  	  if (SYN_assignStatemnet(f)) {
		SEM_assignValue(term_lex) ;
		return true;
	  }    // tu bola zmena
  	  else return false;
  	case KEY_READLN:
  	  SYN_readLexem(f);
  	  if (SYN_readlnStatemnet(f)) return true;
  	  else return false;
  	case KEY_WRITE:
  	  SYN_readLexem(f);
  	  if (SYN_writeStatemnet(f)) return true;
  	  else return false;
  	default:
  	  return false;
  }
}

bool SYN_semicolons(FILE *f){
  
  switch(lexema->type){
  	case KEY_END:
  	  SYN_readLexem(f);
  	  return true;
  	case STREDNIK:
  	  SYN_readLexem(f);	
  	  if (SYN_statement(f) && SYN_semicolons(f)) return true;
  	  else return false;
  	default:
  	  return false;
  }
}

bool SYN_statementList(FILE *f){
  
  switch (lexema->type) {
  	case KEY_END:
	case STREDNIK:                              
	  if (SYN_semicolons(f)) return true;
	  else return false;	  
	case KEY_IF:
	case KEY_WHILE:
	case IDENTIFICATOR:
	case KEY_READLN:
	case KEY_WRITE:
	  if (SYN_statement(f) && SYN_semicolons(f)) return true;
	  else return false; 
	default:
	  return false;	
  } 
}

bool SYN_comStatement(FILE *f){
  if (lexema->type!=KEY_BEGIN) return false;
  SYN_readLexem(f);
  if (SYN_statementList(f)) return true;
  else return false;
}

bool SYN_program(FILE *f){
  switch (lexema->type){
  	case KEY_VAR:
	case KEY_FUNCTION:
	case KEY_BEGIN:
  	  if (!SYN_decPrem(f,GLOBAL) || !SYN_decFunc(f)) return false; 
      BS_checkFunction(pointers->SYM_TABLE);
      if (!SYN_comStatement(f)) return false;
      if (lexema->type!=BODKA) return false;
      SYN_readLexem(f);
      if (lexema->type==TYPE_EOF) return true;
      else return false;
    default:
      return false;
  } 
}

void SYN_parser(FILE *f){
  
  lexema=(PTStructLex)malloc(sizeof(TStructLex));
  if (lexema==NULL) error(ERR_INTERNAL, "Chyba alokacie pamete");
  LEX_getLexem(lexema,f);
  SEM_insertEmbFunc();
  if (!SYN_program(f)) {
    free(lexema->lex);
    free(lexema);
    error(ERR_SYN, "Syntakticka chyba");
  } else {
    free(lexema->lex);
    free(lexema);
    printf("SYNTAX OK.\n"); 
  } 
}
