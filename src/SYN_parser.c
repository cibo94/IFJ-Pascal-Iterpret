#include"SYN_parser.h"
#include"inc.h"
 
PTStructLex lexema;
static bool expression=true;

void SYN_readLexem(FILE *f){

  lexema=(PTStructLex)malloc(sizeof(TStructLex));
  if (lexema==NULL) error(ERR_INTERNAL, "Chyba alokacie pamete");
  LEX_getLexem(lexema,f); 
}

bool SYN_funcBody(FILE *f){

  if (!SYN_comStatement(f)) return false;
  if (lexema->type!=STREDNIK) return false;
  SYN_readLexem(f);
  return true;
}

bool SYN_onlyDecFunc(FILE *f){

  switch(lexema->type){
    case KEY_VAR:
    case KEY_BEGIN:
      if (SYN_decPrem(f) && SYN_funcBody(f) && SYN_decFunc(f)) return true;
      else return false;
    case KEY_FORWARD:
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

  if (lexema->type!=IDENTIFICATOR) return false;
  SYN_readLexem(f);
  if (lexema->type!=DDOT) return false;
  SYN_readLexem(f);
  if (SYN_type(f) && SYN_endParam(f)) return true;
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
  
  switch (lexema->type){
  	case KEY_FUNCTION:
  	  SYN_readLexem(f);
  	  if (lexema->type!=IDENTIFICATOR) return false;
  	  SYN_readLexem(f);
  	  if (lexema->type!=LBRACKET) return false;
  	  SYN_readLexem(f);
  	  if (!SYN_paramList(f)) return false;
  	  if (lexema->type!=RBRACKET) return false;
  	  SYN_readLexem(f);
  	  if (lexema->type!=DDOT) return false;
  	  SYN_readLexem(f);
  	  if (!SYN_type(f)) return false;	
  	  if (lexema->type!=STREDNIK) return false;
  	  SYN_readLexem(f);
  	  if (SYN_onlyDecFunc(f)) return true;
  	  else return false;
    case KEY_BEGIN:
      return true;
    default:
      return false;
  }
}

bool SYN_nextPrem(FILE *f){
  
  switch (lexema->type){
    case KEY_BEGIN:
    case KEY_FUNCTION:
    	return true;
    case IDENTIFICATOR:
    	if (SYN_prem(f) && SYN_nextPrem(f)) return true;
    	else return false;
    default:
    	return false;
  }
}

bool SYN_type(FILE *f){
  
  switch (lexema->type){
    case KEY_INTEGER:
    case KEY_REAL:
    case KEY_STRING:
    case KEY_BOOLEAN:
      SYN_readLexem(f);
      return true;
    default:
      return false;
  }
}

bool SYN_prem(FILE *f){
  
  if (lexema->type!=IDENTIFICATOR) return false;
  SYN_readLexem(f);
  if (lexema->type!=DDOT) return false;
  SYN_readLexem(f);
  if (!SYN_type(f)) return false;
  if (lexema->type!=STREDNIK) return false;
  SYN_readLexem(f);
  return true;
}
bool SYN_decPrem(FILE *f){
  
  switch (lexema->type){
    case KEY_BEGIN:
    case KEY_FUNCTION:
	  return true;
	case KEY_VAR:
	  SYN_readLexem(f);
	  if (SYN_prem(f) && SYN_nextPrem(f)) return true;
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
  if (expression) return true;
  else return false;
}

bool SYN_whileStatemnet(FILE *f){
  
  if (!expression) return false;
  if (lexema->type!=KEY_DO) return false;
  SYN_readLexem(f);
  if (SYN_comStatement(f)) return true;
  else return false;
}

bool SYN_elseStatemnet(FILE *f){
  
  if (lexema->type!=KEY_ELSE) return false;
  SYN_readLexem(f);
  if (SYN_comStatement(f)) return true;
  else return false;
}

bool SYN_ifStatemnet(FILE *f){
  
  if (!expression) return false;
  if (lexema->type!=KEY_THEN) return false;
  SYN_readLexem(f);
  if (SYN_comStatement(f) && SYN_elseStatemnet(f)) return true;
  else return false;
  
}

bool SYN_statement(FILE *f){
  
  switch (lexema->type){
  	case KEY_IF:
  	  SYN_readLexem(f);
  	  if (SYN_ifStatemnet(f)) return true;
  	  else return false;
  	case KEY_WHILE:
  	  SYN_readLexem(f);
  	  if (SYN_whileStatemnet(f)) return true;
  	  else return false;
  	case IDENTIFICATOR:
  	  SYN_readLexem(f);
  	  if (SYN_assignStatemnet(f)) return true;
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
	case DDOT:
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
  	  if (!SYN_decPrem(f) || !SYN_decFunc(f)) return false; 
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
  
  SYN_readLexem(f);
  if (!SYN_program(f)) error(ERR_SYN, "Syntakticka chyba");
  else printf("SYNTAX OK.\n");  
}
