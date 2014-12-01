#include "inc.h"

//  GLOBALNE PREMENNE
     
     extern PGLOB_DEST pointers;
     //extern TTerm embededFunc[];
     
//!< DOPLNENE     
//  FUNKCIE NAD ZASOBNIKOM LABELOV
TSlabelStack SEM_initLS(){
    TSlabelStack stack = malloc(sizeof(struct SlabelStack));
    if(stack == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    stack->top = NULL;
    return stack;
}

void SEM_pushLS(TSlabelStack stack, TTerm * elem){
    TSlabel label = malloc(sizeof(struct Slabel));
    if(label == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    label->data = elem;
    label->previous = stack->top;
    stack->top = label;
}


TTerm * SEM_popLS(TSlabelStack stack){
    if(stack->top == NULL)
        error(ERR_SEM_OTHERS,"Chyba, POP nad prazdnym zasobnikom skokov");
    TSlabel label = stack->top;
    TTerm * toReturn = label->data;
    stack->top = label->previous;
    free(label);
    return toReturn;
}


void SEM_disposeLS(TSlabelStack stack){
    TSlabel label;
    while(stack->top != NULL){
        label = stack->top;
        stack->top = label->previous;
        free(label);
    }
    free(stack);
}
//!< DOPLNENE END     
     
//  FUNKCIE NAD ZASOBNIKOM TYPOV

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
    if(stack->top == NULL)    
        error(ERR_SEM_OTHERS,"Chyba, POP nad prazdnym zasobnikom typov");
    ETermType returnType = stack->top->data;       // ULOZENIE NAVRATOVEHO TYPU
    TStackElem toDelete  = stack->top;             // ULOZENIE PRVKU KTORY SA ZMAZE 
    stack->top = toDelete->previous;               // ZNIZENIE VRCHOLU ZASOBNIKA
    free(toDelete);                                // UVOLNENIE POPNUTEHO PRVKU 
    return returnType;
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
    
    if( (pointers->CURRENTFUNCT->data->flags & LEX_FLAGS_TYPE_FUNC_DEK) != 0 ){                                            // AK SA JEDNA O UZ DEKLAROVANU FUNKCIU
        funcParam = BS_Find(pointers->SCOPE, dataID);
        if(funcParam == NULL) error(ERR_SEM_UNDEF,"Semanticka chyba! Chybne parametre pri deklaracii funkcie\n");   //  AK SA PARAMETER S DANYM ID NENASIEL = CHYBA
        if(funcParam->data->value->value.offset != pointers->PARAMCOUNT)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Chybne parametre pri deklaracii funkcie\n"); // AK SA PARAMETER NASIEL, ALE NESEDI JEHO POZICIA = CHYBA
        switch(dataType->type){                                                                 // AK SA NASIEL A SEDI JEHO POZICIA, ALE NESEDI TYP = CHYBA
            case KEY_INTEGER: if(funcParam->data->value->type != TERM_INT) error(ERR_SEM_TYPE,"Semanticka chyba! Chybne parametre (typ) pri deklaracii funkcie\n");       break;        
            case KEY_STRING:  if(funcParam->data->value->type != TERM_STRING) error(ERR_SEM_TYPE,"Semanticka chyba! Chybne parametre (typ) pri deklaracii funkcie\n");    break;    
            case KEY_REAL:    if(funcParam->data->value->type != TERM_REAL) error(ERR_SEM_TYPE,"Semanticka chyba! Chybne parametre (typ) pri deklaracii funkcie\n");      break;    
            case KEY_BOOLEAN: if(funcParam->data->value->type != TERM_BOOL) error(ERR_SEM_TYPE,"Semanticka chyba! Chybne parametre (typ) pri deklaracii funkcie\n");      break;    
            default : break;
        }
        (pointers->PARAMCOUNT)++;
        return; // INAK OK
    }
    else{                                                                     // AK SA JEDNA O NEDEKLAROVANU FUNKCIU
        funcParam = BS_Find(pointers->SCOPE, dataID);                         // PRIDANIE NOVEHO PRVKU DO PODSTROMU FUNKCIE
        if (funcParam != NULL) error(ERR_SEM_UNDEF,"Semanticka chyba! Identifikator parametra '%s' je pouzity 2 krat\n", dataID->lex);
        funcParam = BS_Add(pointers->SCOPE, dataID);
        funcParam->data->value = malloc(sizeof(struct STerm));
        if(funcParam->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
        
        switch(dataType->type){                                                         // NASTAVENIE DATOVEHO TYPU  A ROZSIRENIE STRINGU FUNKCIE
            case KEY_INTEGER: funcParam->data->value->type = TERM_INT;    LEX_string(&(pointers->CURRENTFUNCT->data->param),'i',&(pointers->PARAMCOUNT));   break;        
            case KEY_STRING:  funcParam->data->value->type = TERM_STRING; LEX_string(&(pointers->CURRENTFUNCT->data->param),'s',&(pointers->PARAMCOUNT));   break;    
            case KEY_REAL:    funcParam->data->value->type = TERM_REAL;   LEX_string(&(pointers->CURRENTFUNCT->data->param),'r',&(pointers->PARAMCOUNT));   break;    
            case KEY_BOOLEAN: funcParam->data->value->type = TERM_BOOL;   LEX_string(&(pointers->CURRENTFUNCT->data->param),'b',&(pointers->PARAMCOUNT));   break;    
            default : break;
        }    
        funcParam->data->value->index = true;                                           // PARAMETER JE INDEXOVY UKAZATEL DO ZASOBNIKA
        funcParam->data->value->value.offset = pointers->PARAMCOUNT;                    // UKAZUJE TAM KAM PARAMCOUNT  
        funcParam->data->flags = LEX_FLAGS_INIT;
        return;       
    }
}

void SEM_defineLocal(PTStructLex dataID, PTStructLex dataType){
    TSbinstrom newNode = BS_Find(pointers->SCOPE, dataID);
    if(newNode != NULL) error(ERR_SEM_UNDEF,"Semanticka chyba! Identifikator lokalnej premennej '%s' uz bol definovany\n", dataID->lex);
    newNode = BS_Add(pointers->SCOPE, dataID);
    
    newNode->data->value = malloc(sizeof(struct STerm));
    if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    
    switch(dataType->type){
        case KEY_INTEGER: newNode->data->value->type = TERM_INT;     LEX_string(&(pointers->CURRENTFUNCT->data->param),'x',&(pointers->PARAMCOUNT));  break;        
        case KEY_STRING:  newNode->data->value->type = TERM_STRING;  LEX_string(&(pointers->CURRENTFUNCT->data->param),'x',&(pointers->PARAMCOUNT));  break;    
        case KEY_REAL:    newNode->data->value->type = TERM_REAL;    LEX_string(&(pointers->CURRENTFUNCT->data->param),'x',&(pointers->PARAMCOUNT));  break;    
        case KEY_BOOLEAN: newNode->data->value->type = TERM_BOOL;    LEX_string(&(pointers->CURRENTFUNCT->data->param),'x',&(pointers->PARAMCOUNT));  break;    
        default : break;
    }
    
    newNode->data->value->index = true;
    newNode->data->value->value.offset = pointers->PARAMCOUNT;
    SEM_generate(OP_PUSH, newNode->data->value, NULL, NULL);            //  PUSH LOKALNEJ NA MIROV ZASOBNIK
    pointers->SCOPE->data->value->value.address = pointers->PARAMCOUNT;
    return;
}

void SEM_defineFunction(PTStructLex dataID){
    TSbinstrom newNode = BS_Find(pointers->SYM_TABLE, dataID);          //  PRI DEFINICII FUNKCIE SA HLADA V GLOBALNEJ TABULKE SYMBOLOV
    pointers->PARAMCOUNT = 0;                                           //  VYNULUJE SA PARAMCOUNT
    
    if(newNode != NULL){                                                //  AK SA NASIEL UZOL S ROVNAKYM ID AKO JE HLADANY(VKLADANY)
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNCTION) == 0)       //  AK NAJDENY UZOL NIE JE FUNKCIA -> ERROR, ID FUNKCIE = GLOBALNEJ PREMENNEJ
            error(ERR_SEM_UNDEF,"Semanticka chyba! Chyba pri definicii funkcie '%s'. Existuje premenna s rovnakym nazvom.\n", dataID->lex);
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNC_DEF) != 0)       //  AK JE NAJDENY UZOL FUNKCIA, KTORA UZ BOLA DEKLAROVANA -> ERROR, REDEFINICIA FUNKCIE
            error(ERR_SEM_UNDEF,"Semanticka chyba! Funkcia '%s' uz bola deklarovana.\n", dataID->lex);
    }
    else {                                                              //  INAK SA VKLADA NOVA FUNKCIA
        newNode = BS_Add(pointers->SYM_TABLE,dataID);                   //  PRIDAVA SA NA GLOBALNU UROVEN      
        newNode->data->param = malloc(32);                              //  PRIPRAVA STRINGU PARAMETROV
        if(newNode->data->param == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");   
        newNode->data->value = malloc(sizeof(struct STerm));            //  PRIPRAVA TERMU, VOLANEJ ADRESY PRI VOLANI FUNKCIE
        if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");
        newNode->data->value->type = TERM_EIP;                          //  NASTAVENIE TYPU ADRESY
        
        newNode->data->flags = LEX_FLAGS_TYPE_FUNCTION;                 //  OZNACENIE ZE SA JEDNA O FUNKCIU
    }
    pointers->CURRENTFUNCT = newNode;               //  NASTAVENIE SUCASNEJ FUNKCIE
    pointers->SCOPE = newNode->loc_table;           //  NASTAVENIE JEJ PODSTROMU
    return;
}

void SEM_defFuntionType(PTStructLex dataType){  
    uint32_t flagy = pointers->CURRENTFUNCT->data->flags;
    
    if((flagy & LEX_FLAGS_TYPE_FUNC_DEK) != 0){
        switch(dataType->type){
            case KEY_INTEGER: if((flagy & LEX_FLAGS_TYPE_INT) == 0)    error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ\n");     break;        
            case KEY_STRING:  if((flagy & LEX_FLAGS_TYPE_STRING) == 0) error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ\n");  break;    
            case KEY_REAL:    if((flagy & LEX_FLAGS_TYPE_REAL) == 0)   error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ\n");    break;    
            case KEY_BOOLEAN: if((flagy & LEX_FLAGS_TYPE_BOOL) == 0)   error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ\n");    break;    
            default : break;
            }
            return;
    }    

    switch(dataType->type){
        case KEY_INTEGER: pointers->CURRENTFUNCT->data->flags = (flagy | LEX_FLAGS_TYPE_INT);      break;        
        case KEY_STRING:  pointers->CURRENTFUNCT->data->flags = (flagy | LEX_FLAGS_TYPE_STRING);   break;    
        case KEY_REAL:    pointers->CURRENTFUNCT->data->flags = (flagy | LEX_FLAGS_TYPE_REAL);     break;   
        case KEY_BOOLEAN: pointers->CURRENTFUNCT->data->flags = (flagy | LEX_FLAGS_TYPE_BOOL);     break;   
        default : break;
    }
    
    pointers->CURRENTFUNCT->data->value->value.address = pointers->PROGRAMINDEX;
    
    return;
}

void SEM_checkFunction(PTStructLex lexema){
    if((lexema->flags & LEX_FLAGS_TYPE_FUNCTION)!=0)
        if((strcmp(lexema->lex, "copy") != 0)&&(strcmp(lexema->lex, "length") != 0)&&((lexema->flags & LEX_FLAGS_TYPE_FUNC_DEF)==0)) // AK NIE JE COPY, NIE JE LENGTH A NIE JE DEFINOVANA
            error(ERR_SEM_UNDEF,"Semanticka chyba! Funkcia '%s' nie je definovana.\n", lexema->lex);
}

void SEM_endFunctionDef(PTStructLex lexema){

    if(lexema->type == KEY_FORWARD)
        if((pointers->CURRENTFUNCT->data->flags & LEX_FLAGS_TYPE_FUNC_DEK) != 0)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Opatovna redeklaracia funkcie '%s'\n", pointers->CURRENTFUNCT->data->lex);

     
    pointers->CURRENTFUNCT->data->flags = (pointers->CURRENTFUNCT->data->flags | LEX_FLAGS_TYPE_FUNC_DEK); 
    if(lexema->type == STREDNIK){
        char * helpParam = pointers->CURRENTFUNCT->data->param;
        char * x;
        
        SEM_generate(OP_POP, NULL, NULL, pointers->ACCREG);
        
        TTerm * pocetParametrov = malloc(sizeof(struct STerm));
        if (pocetParametrov == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
        pocetParametrov->value.integer = ((x = strstr(helpParam, "x")) == NULL)?(strlen(helpParam)):(unsigned)(x - helpParam);
        
        TTerm * pocetLokalnych = malloc(sizeof(struct STerm));
        if (pocetLokalnych == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
        pocetLokalnych->value.integer = ((x = strstr(helpParam, "x")) == NULL)?(0):(pocetParametrov->value.integer -(x - helpParam));
        
        SEM_addCL(pointers->CONSTLIST, pocetLokalnych);
        SEM_addCL(pointers->CONSTLIST, pocetParametrov);
        SEM_generate(OP_RET, pocetLokalnych, pocetParametrov, NULL);
        
        pointers->CURRENTFUNCT->data->flags = (pointers->CURRENTFUNCT->data->flags | LEX_FLAGS_TYPE_FUNC_DEF);
        if((pointers->CURRENTFUNCT->data->flags & LEX_FLAGS_INIT) == 0)
            error(ERR_SEM_OTHERS,"Semanticka chyba! Funkcii '%s' nebol priradeny vystup.\n", pointers->CURRENTFUNCT->data->lex);
    }
    pointers->SCOPE = pointers->SYM_TABLE;
    pointers->CURRENTFUNCT = NULL;
}


void SEM_createLeaf(PTStructLex lexema){
/**
 *  FUNKCIA, KTORA VYTVARA LISTOVY UZOL PRE KAZDY PRVOK VYRAZU. TENTO UZOL OBSAHUJE UKAZATEL NA TERM, V KTOROM SU POTREBNE DATA PRE VYHODNOTENIE VYRAZU
 *  LISTOVY UZOL OBSAHUJE BUD ODKAZ NA TERM PRIAMO V TABULKE SYMBOLOV (GLOBALNE PREMENNE/LOKALNE PREMENNE A PARAMETRE), ALEBO PRIAMO TERM S HODNOTOU KONSTANTNOU.
 */ 
    TSbinstrom node;   
 
    if(lexema->type == IDENTIFICATOR){               // AK JE PRAVIDLO, REDUKUJ IDENTIFIKATOR NA 'E', TAK SA V KAZDOM PRIPADE HLADA POLOZKA V TABULKE SYMBOLOV
        
        node = BS_Find(pointers->SCOPE, lexema);
        if((pointers->SYM_TABLE != pointers->SCOPE) && (node == NULL))
            node = BS_Find(pointers->SYM_TABLE, lexema);
            
        if (node == NULL)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Nedeklarovana premenna '%s'.\n", lexema->lex);
            
        if ((node->data->flags & LEX_FLAGS_TYPE_FUNCTION) != 0)                  // CHYBOVA HLASKA AK SA POUZIL IDENTIFIKATOR FUNKCIE
           error(ERR_SEM_OTHERS,"Semanticka chyba! Nepovolene pouzitie identifikatora funkcie '%s'.\n", lexema->lex);
        
        if (node == pointers->CURRENTFUNCT)
           error(ERR_SEM_OTHERS,"Semanticka chyba! Nepovolene pouzitie identifikatora funkcie '%s'.\n", lexema->lex);
        
        if ((node->data->flags & LEX_FLAGS_INIT) == 0)                           // CHYBOVA HLASKA, VOLA SA NEINICIALIZOVANA PREMENNA
           error(ERR_SEM_UNDEF,"Semanticka chyba! Neinicializovana hodnota '%s'.\n", lexema->lex);        
        
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

void SEM_functionCall(PTStructLex functID){
    TSbinstrom node = BS_Find(pointers->SYM_TABLE, functID);
    if(node == NULL) error(ERR_SEM_UNDEF,"Semanticka chyba! Volanie nedefinovanej funkcie");
    SEM_generate(OP_CALL, node->loc_table->data->value, NULL, NULL);
}


// dorobit push parametrov funkcie

void SEM_assignValue(PTStructLex lexema){
    
    TSbinstrom node = BS_Find(pointers->SCOPE, lexema);  
    if((pointers->SCOPE != pointers->SYM_TABLE)&&(node == NULL))                 
        node = BS_Find(pointers->SYM_TABLE, lexema);      
        
    if (node == NULL)
        error(ERR_SEM_UNDEF,"Semanticka chyba! Nedefinovany identifikator '%s'.", lexema->lex);    
       
    ETermType retType = SEM_popSS(pointers->EXPRSTACK);

    if(node == pointers->CURRENTFUNCT){
        switch(retType){
        case TERM_INT : if((node->data->flags & LEX_FLAGS_TYPE_INT) == 0) error(ERR_SEM_TYPE,"Semanticka chyba! Vysledok pravej strany je ineho typu nez typ '%s'.", lexema->lex); break;
        case TERM_REAL: if((node->data->flags & LEX_FLAGS_TYPE_REAL) == 0) error(ERR_SEM_TYPE,"Semanticka chyba! Vysledok pravej strany je ineho typu nez typ '%s'.", lexema->lex); break;
        case TERM_BOOL: if((node->data->flags & LEX_FLAGS_TYPE_BOOL) == 0) error(ERR_SEM_TYPE,"Semanticka chyba! Vysledok pravej strany je ineho typu nez typ '%s'.", lexema->lex); break;
        case TERM_STRING: if((node->data->flags & LEX_FLAGS_TYPE_STRING) == 0) error(ERR_SEM_TYPE,"Semanticka chyba! Vysledok pravej strany je ineho typu nez typ '%s'.", lexema->lex); break;
        default : break;
        }
    }
    else if(retType != node->data->value->type)
        error(ERR_SEM_TYPE,"Semanticka chyba! Vysledok pravej strany je ineho typu nez typ '%s'.", lexema->lex);   
    
    node->data->flags = LEX_FLAGS_INIT;
    
    if(node != pointers->CURRENTFUNCT){
        SEM_generate(OP_POP, NULL, NULL, pointers->ACCREG);
        SEM_generate(OP_ASSIGN, pointers->ACCREG, NULL, node->data->value);
    }

    return;
}

// FUNKCIE PRE IF-THEN-ELSE STATEMENT
void SEM_thenStat(){
    ETermType condition = SEM_popSS(pointers->EXPRSTACK);
    if(condition != TERM_BOOL)
        error(ERR_SEM_TYPE,"Semanticka chyba! Podmienka v IF nie je typu boolean\n");
        
    SEM_generate(OP_POP, NULL, NULL, pointers->SREG1);
    SEM_generate(OP_NOT, pointers->SREG1, NULL, pointers->ACCREG);
    
    TTerm * labelElse = malloc(sizeof(struct STerm));
    if(labelElse == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    TTerm * labelEnd  = malloc(sizeof(struct STerm));
    if(labelEnd == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    
    labelElse->type = TERM_EIP;
    labelEnd->type = TERM_EIP;
    
    SEM_pushLS(pointers->LABELSTACK, labelElse);
    SEM_addCL(pointers->CONSTLIST, labelElse);
    SEM_pushLS(pointers->LABELSTACK, labelEnd);
    SEM_addCL(pointers->CONSTLIST, labelEnd);
    SEM_generate(OP_JTRUE, pointers->ACCREG, labelElse, NULL);
}


void SEM_elseStat(){
    TTerm * labelEnd = SEM_popLS(pointers->LABELSTACK);
    SEM_generate(OP_JMP, labelEnd, NULL, NULL);
    TTerm * labelElse = SEM_popLS(pointers->LABELSTACK);
    labelElse->value.address = pointers->PROGRAMINDEX;
    SEM_pushLS(pointers->LABELSTACK, labelEnd);
}

void SEM_endIf(){
    TTerm * labelEnd = SEM_popLS(pointers->LABELSTACK);
    labelEnd->value.address = pointers->PROGRAMINDEX;
}


// FUNKCIE PRE WHILE
void SEM_whileStat(){
    TTerm * labelWhile = malloc(sizeof(struct STerm));
    if(labelWhile == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    labelWhile->type = TERM_EIP;
    labelWhile->value.address = pointers->PROGRAMINDEX;
    
    TTerm * labelEnd = malloc(sizeof(struct STerm));
    if(labelEnd == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    labelEnd->type = TERM_EIP;
    
    SEM_addCL(pointers->CONSTLIST, labelWhile);
    SEM_addCL(pointers->CONSTLIST, labelEnd);
}

void SEM_whileBegin(){
    // DOKONCIT!!!
}


// DALSIE FUNKCIE
/*
void SEM_insertEmbFunc(){
    PTStructLex embFunc = malloc(sizeof(PTStructLex));
    PTStructLex paramID = malloc(sizeof(PTStructLex));
    PTStructLex dataType = malloc(sizeof(PTStructLex));
    
    // FUNKCIA LENGTH
        embFunc->value = &EMBlength;
        embFunc->lex = "length";
    SEM_defineFunction(embFunc);   
        paramID->lex = "s";
        dataType->type = KEY_STRING;
    SEM_defineParam(paramID, dataType);
        embFunc->type = KEY_INTEGER;
    SEM_defFuntionType(embFunc);
        embFunc->type = KEY_FORWARD;    // FUNKCIA JE REDEKLAROVATELNA
    SEM_endFunctionDef(embFunc);
    
    // FUNKCIA COPY
        embFunc->value = &EMBcopy;
        embFunc->lex = "copy";
    SEM_defineFunction(embFunc); 
        paramID->lex = "s";
        dataType->type = KEY_STRING;
    SEM_defineParam(paramID, dataType);
        paramID->lex = "i";
        dataType->type = KEY_INTEGER;   
    SEM_defineParam(paramID, dataType);
        paramID->lex = "n";
        dataType->type = KEY_INTEGER;   
    SEM_defineParam(paramID, dataType);
        embFunc->type = KEY_STRING;
    SEM_defFuntionType(embFunc);
        embFunc->type = KEY_FORWARD;     // FUNKCIA JE REDEKLAROVATELNA
    SEM_endFunctionDef(embFunc);
    
    // FUNKCIA FIND
        embFunc->value = &EMBfind;
        embFunc->lex = "find" ;
    SEM_defineFunction(embFunc);
        paramID->lex = "s";
        dataType->type = KEY_STRING;
    SEM_defineParam(paramID, dataType);    
        paramID->lex = "search";
        dataType->type = KEY_STRING;
    SEM_defineParam(paramID, dataType);       
        embFunc->type = KEY_INTEGER;
    SEM_defFuntionType(embFunc);
        embFunc->type = STREDNIK;    // FUNKCIU NIE JE MOZNE REDEKLAROVAT
    SEM_endFunctionDef(embFunc);    
    
    // FUNCKIA SORT
        embFunc->value = &EMBsort;
        embFunc->lex = "sort";
    SEM_defineFunction(embFunc);
        paramID->lex = "s";
        dataType->type = KEY_STRING;
    SEM_defineParam(paramID, dataType);       
        embFunc->type = KEY_STRING;
    SEM_defFuntionType(embFunc);
        embFunc->type = STREDNIK;     // FUNKCIU NIE JE MOZNE REDEKLAROVAT
    SEM_endFunctionDef(embFunc);    
    
    free(embFunc);
    free(paramID);
    free(dataType);
}
*/

void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result){

 // FUNKCIA, KTORA VYTVORI TROJADRESNU INSTRUKCIU A PRIDA JU DO PROGRAMU
  
    P3AC newInstruction = malloc(sizeof(struct S3AC));
    newInstruction->op = operation;
    newInstruction->op1 = op1;
    newInstruction->op2 = op2;
    newInstruction->ret = result;
    free (newInstruction);
    (pointers->PROGRAMINDEX)++;
    return;
    //SEM_addInstruction(newInstruction);
}



