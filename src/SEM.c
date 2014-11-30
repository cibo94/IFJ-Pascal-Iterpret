#include "inc.h"

//  GLOBALNE PREMENNE
     
     extern PGLOB_DEST pointers;
     
//  FUNKCIE NAD ZASOBNIKOM

TSemStack SEM_initSS(){
    TSemStack stack = malloc(sizeof(struct SemStack));
    if(stack == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    stack->top = NULL;
    return stack;
}

void SEM_pushSS(TSemStack stack, ETermType elem){
    TStackElem new = malloc(sizeof(struct StackElem));
    if (new == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    new->previous = stack->top;                     // VRCHOL SA STANE PREDCHODCOM NOVEHO VRCHOLU
    new->data = elem;                               // NASTAVENIE OBSAHU PRVKU ZASOBNIKA
    stack->top = new;                               // NASTAVI SA NOVY VRCHOL
}

ETermType SEM_popSS(TSemStack stack){
    if(stack->top != NULL){        
    ETermType returnType = stack->top->data;       // ULOZENIE NAVRATOVEHO TYPU
    TStackElem toDelete  = stack->top;             // ULOZENIE PRVKU KTORY SA ZMAZE 
    stack->top = toDelete->previous;               // ZNIZENIE VRCHOLU ZASOBNIKA
    free(toDelete);                                // UVOLNENIE POPNUTEHO PRVKU 
    return returnType;
    }
    return 0;
}

void SEM_disposeSS(TSemStack stack){
    TStackElem toDelete;                   // POMOCNY PTR
    while(stack->top != NULL){             // KYM NIE JE ZASOBNIK PRAZDNY, TAK RUSI VRCHOL
        toDelete = stack->top;
        stack->top = toDelete->previous;
        free(toDelete);
    }
    free(stack);
}

// FUNKCIE PRE ZOZNAM KONSTANT

TSconstList SEM_initCL(){
    TSconstList newList = malloc(sizeof(struct SconstList));
    if (newList == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    newList->first = NULL;
    return newList;
}

void SEM_addCL(TSconstList list, TTerm * elem){
    TSlistElem newElem = malloc(sizeof(struct SlistElem));
    if (newElem == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    newElem->constTerm = elem;
    newElem->next = list->first;
    list->first = newElem;
}

void SEM_disposeCL(TSconstList list){
    TSlistElem toDelete;
    
    while (list->first != NULL){
        toDelete = list->first;
        list->first = toDelete->next;
        free(toDelete->constTerm);
        free(toDelete);
    }
    free(list);
}

//  FUNKCIE POTREBNE PRI DEFINICII A DEKLARACII PREMENNYCH A FUNKCII

void SEM_defineGlobal(PTStructLex dataID, PTStructLex dataType){
    TSbinstrom newNode = BS_Find(pointers->SYM_TABLE, dataID);
    if(newNode != NULL) error(ERR_SEM_UNDEF,"Semanticka chyba! Identifikator '%s' uz bol definovany\n", dataID->lex);
    newNode = BS_Add(pointers->SYM_TABLE, dataID);
    
    newNode->data->value = malloc(sizeof(struct STerm));
    if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    
    switch(dataType->type){
        case KEY_INTEGER: newNode->data->value->type = TERM_INT;       break;        
        case KEY_STRING:  newNode->data->value->type = TERM_STRING;    break;    
        case KEY_REAL:    newNode->data->value->type = TERM_REAL;      break;    
        case KEY_BOOLEAN: newNode->data->value->type = TERM_BOOL;      break;    
        default : break;
    }

    newNode->data->value->index = false;
}

void SEM_defineParam(PTStructLex dataID, PTStructLex dataType){
    TSbinstrom funcParam;
    
    if( (pointers->SCOPE->data->flags & LEX_FLAGS_TYPE_FUNC_DEF) == 0 ){                                            // AK SA JEDNA O UZ DEFINOVANU FUNKCIU
        funcParam = BS_Find(pointers->SCOPE->loc_table, dataID);
        if(funcParam == NULL) error(ERR_SEM_UNDEF,"Semanticka chyba! Chybne parametre pri deklaracii funkcie\n");   //  AK SA PARAMETER S DANYM ID NENASIEL = CHYBA
        if(funcParam->data->value->value.offset != pointers->PARAMCOUNT)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Chybne parametre pri deklaracii funkcie\n"); // AK SA PARAMETER NASIEL, ALE NESEDI JEHO POZICIA = CHYBA
        switch(dataType->type){                                                                 // AK SA NASIEL A SEDI JEHO POZICIA, ALE NESEDI TYP = CHYBA
            case KEY_INTEGER: if(funcParam->data->value->type != TERM_INT) error(ERR_SEM_UNDEF,"Semanticka chyba! Chybne parametre pri deklaracii funkcie\n");       break;        
            case KEY_STRING:  if(funcParam->data->value->type != TERM_STRING) error(ERR_SEM_UNDEF,"Semanticka chyba! Chybne parametre pri deklaracii funkcie\n");    break;    
            case KEY_REAL:    if(funcParam->data->value->type != TERM_REAL) error(ERR_SEM_UNDEF,"Semanticka chyba! Chybne parametre pri deklaracii funkcie\n");      break;    
            case KEY_BOOLEAN: if(funcParam->data->value->type != TERM_BOOL) error(ERR_SEM_UNDEF,"Semanticka chyba! Chybne parametre pri deklaracii funkcie\n");      break;    
            default : break;
        }
        (pointers->PARAMCOUNT)++;
        return; // INAK OK
    }
    else{                                                                               // AK SA JEDNA O NEDEFINOVANU FUNKCIU
        funcParam = BS_Add(pointers->SCOPE->loc_table, dataID);                         // PRIDANIE NOVEHO PRVKU DO PODSTROMU FUNKCIE
        switch(dataType->type){                                                         // NASTAVENIE DATOVEHO TYPU  A ROZSIRENIE STRINGU FUNKCIE
            case KEY_INTEGER: funcParam->data->value->type = TERM_INT;    LEX_string(&(pointers->SCOPE->data->param),'i',&(pointers->PARAMCOUNT));   break;        
            case KEY_STRING:  funcParam->data->value->type = TERM_STRING; LEX_string(&(pointers->SCOPE->data->param),'s',&(pointers->PARAMCOUNT));   break;    
            case KEY_REAL:    funcParam->data->value->type = TERM_REAL;   LEX_string(&(pointers->SCOPE->data->param),'r',&(pointers->PARAMCOUNT));   break;    
            case KEY_BOOLEAN: funcParam->data->value->type = TERM_BOOL;   LEX_string(&(pointers->SCOPE->data->param),'b',&(pointers->PARAMCOUNT));   break;    
            default : break;
        }    
        funcParam->data->value->index = true;                                           // PARAMETER JE INDEXOVY UKAZATEL DO ZASOBNIKA
        funcParam->data->value->value.offset = pointers->PARAMCOUNT;                    // UKAZUJE TAM KAM PARAMCOUNT                                                    
        return;       
    }
}

void SEM_defineLocal(PTStructLex dataID, PTStructLex dataType){
    TSbinstrom newNode = BS_Find(pointers->SCOPE->loc_table, dataID);
    if(newNode != NULL) error(ERR_SEM_UNDEF,"Semanticka chyba! Identifikator lokalnej premennej '%s' uz bol definovany\n", dataID->lex);
    newNode = BS_Add(pointers->SCOPE->loc_table, dataID);
    
    newNode->data->value = malloc(sizeof(struct STerm));
    if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    
    switch(dataType->type){
        case KEY_INTEGER: newNode->data->value->type = TERM_INT;       break;        
        case KEY_STRING:  newNode->data->value->type = TERM_STRING;    break;    
        case KEY_REAL:    newNode->data->value->type = TERM_REAL;      break;    
        case KEY_BOOLEAN: newNode->data->value->type = TERM_BOOL;      break;    
        default : break;
    }

    newNode->data->value->index = true;
    newNode->data->value->value.offset = pointers->PARAMCOUNT;
    (pointers->PARAMCOUNT)++;
    return;
}

void SEM_defineFunction(PTStructLex dataID){
    TSbinstrom newNode = BS_Find(pointers->SYM_TABLE, dataID);
    pointers->PARAMCOUNT = 0;
    
    if(newNode != NULL){
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNCTION) == 0)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Chyba pri definicii funkcie '%s'. Existuje premenna s rovnakym nazvom.\n", dataID->lex);
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNC_DEK) != 0)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Funkcia '%s' uz bola deklarovana.\n", dataID->lex);
    }
    else {
        newNode = BS_Add(pointers->SYM_TABLE,dataID);
        newNode->data->value = malloc(sizeof(struct STerm));     
        if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");
        newNode->data->param = malloc(32);                       
        if(newNode->data->param == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");
        newNode->data->flags = LEX_FLAGS_TYPE_FUNCTION;
    }
    pointers->SCOPE = newNode;
    return;
}

void SEM_defFuntionType(PTStructLex dataType){  
    if((pointers->SCOPE->data->flags & LEX_FLAGS_TYPE_FUNC_DEF) != 0){
        switch(dataType->type){
            case KEY_INTEGER: if(pointers->SCOPE->data->value->type != TERM_INT); error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ\n");     break;        
            case KEY_STRING:  if(pointers->SCOPE->data->value->type != TERM_STRING); error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ\n");  break;    
            case KEY_REAL:    if(pointers->SCOPE->data->value->type != TERM_REAL); error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ\n");    break;    
            case KEY_BOOLEAN: if(pointers->SCOPE->data->value->type != TERM_BOOL); error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ\n");    break;    
            default : break;
            return;
    }    

    switch(dataType->type){
        case KEY_INTEGER: pointers->SCOPE->data->value->type = TERM_INT;       break;        
        case KEY_STRING:  pointers->SCOPE->data->value->type = TERM_STRING;    break;    
        case KEY_REAL:    pointers->SCOPE->data->value->type = TERM_REAL;      break;    
        case KEY_BOOLEAN: pointers->SCOPE->data->value->type = TERM_BOOL;      break;    
        default : break;
    }
    return;
}

void SEM_endFunctionDef(PTStructLex lexema){
    pointers->SCOPE->data->flags = (pointers->SCOPE->data->flags | LEX_FLAGS_TYPE_FUNC_DEF); 
    if(lexema->type == STREDNIK)
        pointers->SCOPE->data->flags = (pointers->SCOPE->data->flags | LEX_FLAGS_TYPE_FUNC_DEK);

    pointers->SCOPE = pointers->SYM_TABLE;
}


void SEM_createLeaf(PTStructLex lexema){
/**
 *  FUNKCIA, KTORA VYTVARA LISTOVY UZOL PRE KAZDY PRVOK VYRAZU. TENTO UZOL OBSAHUJE UKAZATEL NA TERM, V KTOROM SU POTREBNE DATA PRE VYHODNOTENIE VYRAZU
 *  LISTOVY UZOL OBSAHUJE BUD ODKAZ NA TERM PRIAMO V TABULKE SYMBOLOV (GLOBALNE PREMENNE/LOKALNE PREMENNE A PARAMETRE), ALEBO PRIAMO TERM S HODNOTOU KONSTANTNOU.
 */ 

    if(lexema->type == IDENTIFICATOR){               // AK JE PRAVIDLO, REDUKUJ IDENTIFIKATOR NA 'E', TAK SA V KAZDOM PRIPADE HLADA POLOZKA V TABULKE SYMBOLOV
        
        TSbinstrom node = BS_Find(pointers->SCOPE->loc_table, lexema); // mozna chyba ->loc_table
        
        if((pointers->SCOPE != pointers->SYM_TABLE)&&(node == NULL))                 
            node = BS_Find(pointers->SYM_TABLE, lexema);      
            
        if (node == NULL)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Nedefinovana premenna '%s' vo vyraze.\n", lexema->lex);
            
        if ((node->data->flags & LEX_FLAGS_TYPE_FUNCTION) != 0)                  // CHYBOVA HLASKA AK SA POUZIL IDENTIFIKATOR FUNKCIE
           error(ERR_SEM_OTHERS,"Semanticka chyba! Identifikator funkcie '%s' vo vyraze.\n", lexema->lex);
           
        if ((node->data->flags & LEX_FLAGS_INIT) == 0)                           // CHYBOVA HLASKA, VOLA SA NEINICIALIZOVANA PREMENNA
           error(ERR_SEM_UNDEF,"Semanticka chyba! Neinicializovana premenna '%s' vo vyraze.\n", lexema->lex);        
        
        SEM_pushSS(pointers->EXPRSTACK, node->data->value->type);        // ULOZENIE TYPU
        SEM_generate(OP_PUSH, node->data->value, NULL, NULL);
    }
    else{                                                            // V PRIPADE KONSTANTY JU POTREBUJEM ULOZIT DO STROMU AKO GENEROVANU PREMENNU   
        struct STerm * term = malloc(sizeof(struct STerm));          // PRE KONSTANTY SA VYTVARA NOVE UMIESTNENIE, A UKLADA SA DO ZOZNAMU KONSTANT
        if (term == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");
        switch(lexema->type){                                        // PODLA TYPU LEXEMY SA SKONVERTUJE STRING NA POZADOVANU HODNOTU
        
            case INT_CONST    :
                term->value.integer = atoi(lexema->lex);
                term->type = TERM_INT;
            break;
            
            case REAL_CONST   :
                term->value.real = atof(lexema->lex);
                term->type = TERM_REAL;
            break;
            
            case STRING_CONST :
                term->value.string = lexema->lex;
                term->type = TERM_STRING;
            break;
            
            case KEY_TRUE   :
                term->value.boolean = true;
                term->type = TERM_BOOL;
            break;
            
            case KEY_FALSE  :
                term->value.boolean = false;
                term->type = TERM_BOOL;
            break;
            
            default : error(ERR_SEM_OTHERS,"Nie tak celkom semanticka chyba. Dostalo sa mi sem '%s' namiesto nejakeho termu.\n", lexema->lex); break;
        }

        SEM_pushSS(pointers->EXPRSTACK, term->type);     // ULOZENIE TYPU NA SEMANTICKY ZASOBNIK
        SEM_generate(OP_PUSH, term, NULL, NULL);
        SEM_addCL(pointers->CONSTLIST,term);                                 // ULOZENIE KONSTANTY DO ZOZNAMU KONSTANT
        return;
    }
 
 }

void SEM_createTree(PTStructLex lexema){
    
    E_OP relOperator = 0;
    ETermType typeRight = SEM_popSS(pointers->EXPRSTACK);
    ETermType typeLeft  = SEM_popSS(pointers->EXPRSTACK);
    
    if(typeLeft != typeRight)
        error(ERR_SEM_TYPE,"Nekompatibilne datove typy vo vyraze.");
    
    pointers->ACCREG->type = typeRight;
    
    SEM_generate(OP_POP, NULL, NULL, pointers->SREG1);
    SEM_generate(OP_POP, NULL, NULL, pointers->SREG2);
    
    switch(lexema->type){
        case OPERATOR_PLUS     :
            SEM_generate(OP_PLUS, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);         
        break;                                                                          
        
        case OPERATOR_MINUS    :
            if(typeRight != TERM_STRING)
                SEM_generate(OP_MINUS, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);
            else 
                error(ERR_SEM_TYPE, "Semanticka chyba! Retazce nie je mozne odcitat");

        break;
        
        case OPERATOR_TIMES    :
            if(typeRight != TERM_STRING)
                SEM_generate(OP_MUL, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);
            else 
                error(ERR_SEM_TYPE, "Semanticka chyba! Retazce nie je mozne nasobit");
        break;
        
        case OPERATOR_DIV      :
            if(typeRight != TERM_STRING)
                SEM_generate(OP_DIV, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);
            else 
                error(ERR_SEM_TYPE, "Semanticka chyba! Retazec nie je mozne delit retazcom");
        break;
        
        case OPERATOR_GREATER  : relOperator = OP_GREAT;   break;
        case OPERATOR_SMALLER  : relOperator = OP_LESS;    break;
        case OPERATOR_GREATEQ  : relOperator = OP_GREATEQ; break;
        case OPERATOR_SMALLEQ  : relOperator = OP_LESSEQ;  break;
        case OPERATOR_EQUAL    : relOperator = OP_EQUAL;   break;
        case OPERATOR_NEQUAL   : relOperator = OP_NEQUAL;  break;    
        default             :   break;
        }     
    
        if(relOperator != 0){
            pointers->ACCREG->type = TERM_BOOL;
            SEM_generate(relOperator, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);  
        }
    
        SEM_pushSS(pointers->EXPRSTACK, pointers->ACCREG->type);
        SEM_generate(OP_PUSH, pointers->ACCREG, NULL, NULL);
}
/*
void SEM_assignValue(){
    
    TSbinstrom node = BS_Find(GLOBAL_scope, lexema);
        
    if((GLOBAL_scope != GLOBAL_sym_table)&&(node == NULL))                 
        node = BS_Find(GLOBAL_sym_table, lexema);      
        
    if (node == NULL)
        error(ERR_SEM,"Nedefinovany identifikator '%s'", lexema->lex);    
    
    SEM_generate(OP_POP, GLOBAL_SEM_SourceOne, NULL, NULL);
    ETermType retType = SEM_popSS(GLOBAL_SEM_ExpStack);
    
    if(node->data->value->type != retType)
        error(ERR_SEM,"Nekompatibilne typy priradenia");
        
    SEM_generate(OP_ASSIGN, GLOBAL_SEM_SourceOne, NULL, node->data->value)
    
    return;
}

void SEM_functionCall(int *paramCounter, PTStructLex functionLexem){
    
    TSbinstrom fNode = BS_Find(GLOBAL_sym_table, functionLexem);
    if(*paramCounter != strlen(fNode->data->param) )
        error(ERR_SEM,"Chybny pocet parametrov pri volani funkcie %s", functionLexem->lex);
    
    //generate(OP_CALL, fNode->data)
    // SKOK
    return;
}

void SEM_functionParam(int *paramCounter, TSbinstrom functionNode){

//  VOLA SA PRI SPRACOVAVANI PARAMETROV VOLANEJ FUNKCIE
//  V TERME FUNKCIE, KONKRETNE VO VALUE.STRING JE STRING OBSAHUJUCI PARAMETRE FUNKCIE
 
 
    if((*paramCounter+1) > strlen(functionNode->data->param))
        error(ERR_SEM,"Prilis vela parametrov funkcie");
    
    variableNode = BS_Find(GLOBAL_scope, lexema);
    if ((variableNode == NULL)&&(GLOBAL_scope != GLOBAL_sym_table))
        variableNode = BS_Find(GLOBAL_sym_table, lexema);
        
    if(variableNode == NULL);
        error(ERR_SEM,"Premenna %s nie je definovana", lexema->lex);
       
    uint32_t flags = variableNode->data->flags
    if (flags & LEX_FLAGS_TYPE_FUNCTION != 0)
        error(ERR_SEM,"Argumentom funkcie nemoze byt identifikator funkcie");
    if (flags & LEX_FLAGS_INIT  == 0)
        error(ERR_SEM,"Argument funkcie nie je inicializovany");
    
    char cTyp;
    
    if(variableNode->data->value->type == TERM_INT)            // NASTAVENIE SPRAVNEHO TYPU MEDZIVYSLEDKOVEHO TERMU 
        cTyp = 'i';
    if(variableNode->data->value->type == TERM_REAL)
        cTyp = 'r';
    if(variableNode->data->value->type == TERM_BOOL)
        cTyp = 'b';
    if(variableNode->data->value->type == TERM_STRING)
        cTyp = 's';
    
    if(cTyp != functionNode->data->param[*paramCounter])
        error(ERR_SEM,"Argument %s je nespravneho typu", lexema->lex);
        
    SEM_generate(OP_PUSH, variableNode->data->value, NULL, NULL);
    *paramCounter++;
    return;
}

//  DALSIE FUNKCIE

*/
void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result){

 // FUNKCIA, KTORA VYTVORI TROJADRESNU INSTRUKCIU A PRIDA JU DO PROGRAMU
  
    P3AC newInstruction = malloc(sizeof(struct S3AC));
    newInstruction->op = operation;
    newInstruction->op1 = op1;
    newInstruction->op2 = op2;
    newInstruction->ret = result;
    free (newInstruction);
    return;
    //SEM_addInstruction(newInstruction);
}



