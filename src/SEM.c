#include "inc.h"

//!<  GLOBALNE PREMENNE
     
     extern PGLOB_DEST pointers;
     //extern TTerm embededFunc[];
     
   
//!<  FUNKCIE NAD ZASOBNIKOM LABELOV
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
 
     
//!<  FUNKCIE NAD ZASOBNIKOM TYPOV
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


//!< FUNKCIE PRE ZOZNAM KONSTANT
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


//!<  FUNKCIE POTREBNE PRI DEFINICII A DEKLARACII PREMENNYCH A FUNKCII
void SEM_defineGlobal(PTStructLex dataID, PTStructLex dataType){
    TSbinstrom newNode = BS_Find(pointers->SYM_TABLE, dataID);
    if(newNode != NULL) error(ERR_SEM_UNDEF,"Identifikator '%s' uz bol definovany\n", dataID->lex);
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
        if(funcParam == NULL) error(ERR_SEM_UNDEF,"Chybne parametre  pri deklaracii funkcie\n");   //  AK SA PARAMETER S DANYM ID NENASIEL = CHYBA
        if(funcParam->data->value->value.offset != pointers->PARAMCOUNT)
            error(ERR_SEM_UNDEF,"Chybne parametre pri deklaracii funkcie\n"); // AK SA PARAMETER NASIEL, ALE NESEDI JEHO POZICIA = CHYBA
        switch(dataType->type){                                                                 // AK SA NASIEL A SEDI JEHO POZICIA, ALE NESEDI TYP = CHYBA
            case KEY_INTEGER: if(funcParam->data->value->type != TERM_INT) error(ERR_SEM_TYPE,"Chybne parametre pri deklaracii funkcie\n");       break;        
            case KEY_STRING:  if(funcParam->data->value->type != TERM_STRING) error(ERR_SEM_TYPE,"Chybne parametre pri deklaracii funkcie\n");    break;    
            case KEY_REAL:    if(funcParam->data->value->type != TERM_REAL) error(ERR_SEM_TYPE,"Chybne parametre pri deklaracii funkcie\n");      break;    
            case KEY_BOOLEAN: if(funcParam->data->value->type != TERM_BOOL) error(ERR_SEM_TYPE,"Chybne parametre pri deklaracii funkcie\n");      break;    
            default : break;
        }
        (pointers->PARAMCOUNT)++;
        return; // INAK OK
    }
    else{    // AK SA JEDNA O NEDEKLAROVANU FUNKCIU
        if(strcmp(dataID->lex,pointers->CURRENTFUNCT->data->lex) == 0)
            error(ERR_SEM_UNDEF,"Parameter funkcie nemoze mat rovnaky nazov ako funkcia\n");
            
        funcParam = BS_Find(pointers->SCOPE, dataID);                         // PRIDANIE NOVEHO PRVKU DO PODSTROMU FUNKCIE
        if (funcParam != NULL) error(ERR_SEM_UNDEF,"Identifikator parametra '%s' je pouzity 2 krat\n", dataID->lex);
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
        funcParam->data->value->value.offset = (pointers->PARAMCOUNT) - 1;                    // UKAZUJE TAM KAM PARAMCOUNT  
        funcParam->data->flags = LEX_FLAGS_INIT;
        return;       
    }
}


void SEM_defineLocal(PTStructLex dataID, PTStructLex dataType){
    if(strcmp(dataID->lex,pointers->CURRENTFUNCT->data->lex) == 0)
        error(ERR_SEM_UNDEF,"Parameter funkcie nemoze mat rovnaky nazov ako funkcia\n");

    TSbinstrom newNode = BS_Find(pointers->SCOPE, dataID);
    if(newNode != NULL) error(ERR_SEM_UNDEF,"Identifikator lokalnej premennej '%s' uz bol definovany\n", dataID->lex);
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
            error(ERR_SEM_UNDEF,"Chyba pri definicii funkcie '%s'. Existuje premenna s rovnakym nazvom.\n", dataID->lex);
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNC_DEF) != 0)       //  AK JE NAJDENY UZOL FUNKCIA, KTORA UZ BOLA DEKLAROVANA -> ERROR, REDEFINICIA FUNKCIE
            error(ERR_SEM_UNDEF,"Funkcia '%s' uz bola deklarovana.\n", dataID->lex);
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
    
    if(strcmp(dataID->lex, "find")==0)
        newNode->data->flags = (newNode->data->flags | LEX_FLAGS_INIT);
    if(strcmp(dataID->lex, "sort")==0)
        newNode->data->flags = (newNode->data->flags | LEX_FLAGS_INIT);
    if(strcmp(dataID->lex, "copy")==0)
        newNode->data->flags = (newNode->data->flags | LEX_FLAGS_INIT);
    if(strcmp(dataID->lex, "length")==0)
        newNode->data->flags = (newNode->data->flags | LEX_FLAGS_INIT);
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
            error(ERR_SEM_UNDEF,"Funkcia '%s' nie je definovana.\n", lexema->lex);
}


void SEM_endFunctionDef(PTStructLex lexema){

    if(lexema->type == KEY_FORWARD)
        if((pointers->CURRENTFUNCT->data->flags & LEX_FLAGS_TYPE_FUNC_DEK) != 0)
            error(ERR_SEM_UNDEF,"Opatovna redeklaracia funkcie '%s'\n", pointers->CURRENTFUNCT->data->lex);

     
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
            error(ERR_SEM_OTHERS,"Funkcii '%s' nebol priradeny vystup.\n", pointers->CURRENTFUNCT->data->lex);
    }
    
    pointers->SCOPE = pointers->SYM_TABLE;
    pointers->CURRENTFUNCT = NULL;
}


//!< FUNKCIE NA SPRACOVANIE VYRAZOV
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
            error(ERR_SEM_UNDEF,"Nedeklarovana premenna '%s'.\n", lexema->lex);
            
        if ((node->data->flags & LEX_FLAGS_TYPE_FUNCTION) != 0)                  // CHYBOVA HLASKA AK SA POUZIL IDENTIFIKATOR FUNKCIE
           error(ERR_SEM_OTHERS,"Nepovolene pouzitie identifikatora funkcie '%s'.\n", lexema->lex);
        
        if (node == pointers->CURRENTFUNCT)
           error(ERR_SEM_OTHERS,"Nepovolene pouzitie identifikatora funkcie '%s'.\n", lexema->lex);
        
        if ((node->data->flags & LEX_FLAGS_INIT) == 0)                           // CHYBOVA HLASKA, VOLA SA NEINICIALIZOVANA PREMENNA
           error(ERR_SEM_UNDEF,"Neinicializovana hodnota '%s'.\n", lexema->lex);        
        
        SEM_pushSS(pointers->EXPRSTACK, node->data->value->type);        // ULOZENIE TYPU
        SEM_generate(OP_PUSH, node->data->value, NULL, NULL);
    }
    else{                                                            // V PRIPADE KONSTANTY JU POTREBUJEM ULOZIT DO STROMU AKO GENEROVANU PREMENNU   
        struct STerm * term = malloc(sizeof(struct STerm));          // PRE KONSTANTY SA VYTVARA NOVE UMIESTNENIE, A UKLADA SA DO ZOZNAMU KONSTANT
        if (term == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");
        switch(lexema->type){                                        // PODLA TYPU LEXEMY SA SKONVERTUJE STRING NA POZADOVANU HODNOTU
            case INT_CONST    :  term->value.integer = atoi(lexema->lex);
                                 term->type = TERM_INT; break;
            case REAL_CONST   :  term->value.real = atof(lexema->lex);
                                 term->type = TERM_REAL; break;  
            case STRING_CONST :  term->value.string = lexema->lex;
                                 term->type = TERM_STRING; break;           
            case KEY_TRUE   :    term->value.boolean = true;
                                 term->type = TERM_BOOL; break;           
            case KEY_FALSE  :    term->value.boolean = false;
                                 term->type = TERM_BOOL; break;
            default : break;
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
                error(ERR_SEM_TYPE, "Retazce nie je mozne odcitat");

        break;
        
        case OPERATOR_TIMES    :
            if(typeRight != TERM_STRING)
                SEM_generate(OP_MUL, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);
            else 
                error(ERR_SEM_TYPE, "Retazce nie je mozne nasobit");
        break;
        
        case OPERATOR_DIV      :
            if(typeRight != TERM_STRING)
                SEM_generate(OP_DIV, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);
            else 
                error(ERR_SEM_TYPE, "Retazec nie je mozne delit retazcom");
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


//!< VOLANIE FUNKCII
void SEM_fCallPrologue(PTStructLex functID){
    TSbinstrom fNode = BS_Find(pointers->SYM_TABLE, functID);
    if(fNode == NULL) error(ERR_SEM_UNDEF,"Volanie nedefinovanej funkcie '%s'.\n", functID->lex);
    
    pointers->PARAMCOUNT = 0;            //  VYNULOVANIE POCITADLA PARAMETROV
}


void SEM_functionCall(PTStructLex functID){
    TSbinstrom node = BS_Find(pointers->SYM_TABLE, functID);
    SEM_generate(OP_CALL, node->data->value, NULL, NULL);                       //  SKOK NA FUNKCIU
}


void SEM_functionParam(PTStructLex functID, PTStructLex paramID){
    TSbinstrom fNode = BS_Find(pointers->SYM_TABLE, functID);                   //  FUNKCIA SA NAJDE V TABULKE SYMBOLOV
    
    
    if(paramID->type == IDENTIFICATOR){
        TSbinstrom pNode = BS_Find(pointers->SCOPE, paramID);                       //  PARAMETER SA HLADA V SCOPE
        if((pNode == NULL)&&(pointers->SYM_TABLE != pointers->SCOPE))               //  AK NIE JE V SCOPE TAK V TABULKE SYMBOLOV
            pNode = BS_Find(pointers->SYM_TABLE, paramID);
        
        if(pNode == NULL) error(ERR_SEM_UNDEF,"Nedefinovany parameter funkcie '%s'.\n", paramID->lex);  //  AK SA NENAJDE TAK JE NEDEFINOVANY
    
        if((pNode->data->flags & LEX_FLAGS_TYPE_FUNCTION) != 0) error(ERR_SEM_UNDEF,"Parametrom funkcie je identifikator funkcie '%s'.\n", paramID->lex);    // AK SA NAJDE A JE TO IDENTIFIKATOR FUNKCIE TAK ERROR
    
        switch(pNode->data->value->type){   // INAK SA ROBI TYPOVA KONTROLA
            case TERM_INT   : if((fNode->data->param)[pointers->PARAMCOUNT] != 'i') error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT, functID->lex);break;
            case TERM_REAL  : if((fNode->data->param)[pointers->PARAMCOUNT] != 'r') error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT, functID->lex);break;
            case TERM_STRING: if((fNode->data->param)[pointers->PARAMCOUNT] != 's') error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT, functID->lex);break;
            case TERM_BOOL  : if((fNode->data->param)[pointers->PARAMCOUNT] != 'b') error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT, functID->lex);break;
            default : break;
        }

        SEM_generate(OP_PUSH, pNode->data->value, NULL, NULL);  // PUSH PARAMETRA NA ZASOBNIK
    }
    else{                                                            // V PRIPADE KONSTANTY JU POTREBUJEM ULOZIT DO STROMU AKO GENEROVANU PREMENNU   
        struct STerm * term = malloc(sizeof(struct STerm));          // PRE KONSTANTY SA VYTVARA NOVE UMIESTNENIE, A UKLADA SA DO ZOZNAMU KONSTANT
        if (term == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");
        switch(paramID->type){                                        // PODLA TYPU LEXEMY SA SKONVERTUJE STRING NA POZADOVANU HODNOTU
            case INT_CONST    :  term->value.integer = atoi(paramID->lex);
                                 term->type = TERM_INT; break;
            case REAL_CONST   :  term->value.real = atof(paramID->lex);
                                 term->type = TERM_REAL; break;  
            case STRING_CONST :  term->value.string = paramID->lex;
                                 term->type = TERM_STRING; break;           
            case KEY_TRUE   :    term->value.boolean = true;
                                 term->type = TERM_BOOL; break;           
            case KEY_FALSE  :    term->value.boolean = false;
                                 term->type = TERM_BOOL; break;
            default : break;
        }
        
        switch(term->type){   // INAK SA ROBI TYPOVA KONTROLA
            case TERM_INT   : if((fNode->data->param)[pointers->PARAMCOUNT] != 'i') error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT, functID->lex);break;
            case TERM_REAL  : if((fNode->data->param)[pointers->PARAMCOUNT] != 'r') error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT, functID->lex);break;
            case TERM_STRING: if((fNode->data->param)[pointers->PARAMCOUNT] != 's') error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT, functID->lex);break;
            case TERM_BOOL  : if((fNode->data->param)[pointers->PARAMCOUNT] != 'b') error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT, functID->lex);break;
            default : break;
        }
        
        SEM_generate(OP_PUSH, term, NULL, NULL);                             // PUSH PARAMETRA
        SEM_addCL(pointers->CONSTLIST,term);                                 // ULOZENIE KONSTANTY DO ZOZNAMU KONSTANT
        return;
    }
    
    (pointers->PARAMCOUNT)++;           // ZVYSENIE POCITADLA PARAMETROV
}


//!< PRIRADENIE
void SEM_assignValue(PTStructLex lexema){
    
    TSbinstrom node = BS_Find(pointers->SCOPE, lexema);                             // V TABULKE SYMBOLOV NAJDE LAVU STRANU VYRAZU 
    if((pointers->SCOPE != pointers->SYM_TABLE)&&(node == NULL))                 
        node = BS_Find(pointers->SYM_TABLE, lexema);      
    
    if (node == NULL)                                                               // AK SA NENASLO, CHYBA
        error(ERR_SEM_UNDEF,"Nedefinovany identifikator '%s'.", lexema->lex);    
       
    ETermType retType = SEM_popSS(pointers->EXPRSTACK);                             // VYSLEDOK PRAVEJ STRANY JE NA VRCHOLE ZASOBNIKA

    if(node == pointers->CURRENTFUNCT){                                             // AK JE LAVA STRANA ROVNA SUCASNEJ FUNKCII, TAK SA KONTROLUJE TYP
        switch(retType){
        case TERM_INT : if((node->data->flags & LEX_FLAGS_TYPE_INT) == 0) error(ERR_SEM_TYPE,"Typ pravej strany je iny nez typ lavej strany '%s'.", lexema->lex); break;
        case TERM_REAL: if((node->data->flags & LEX_FLAGS_TYPE_REAL) == 0) error(ERR_SEM_TYPE,"Typ pravej strany je iny nez typ lavej strany '%s'.", lexema->lex); break;
        case TERM_BOOL: if((node->data->flags & LEX_FLAGS_TYPE_BOOL) == 0) error(ERR_SEM_TYPE,"Typ pravej strany je iny nez typ lavej strany '%s'.", lexema->lex); break;
        case TERM_STRING: if((node->data->flags & LEX_FLAGS_TYPE_STRING) == 0) error(ERR_SEM_TYPE,"Typ pravej strany je iny nez typ lavej strany '%s'.", lexema->lex); break;
        default : break;
        }
    }
    else {
        if ((node->data->flags & LEX_FLAGS_TYPE_FUNCTION) != 0)                    // AK NASLO FUNKCIU, ALE NIE SUCASNU, TAK CHYBA
        error(ERR_SEM_UNDEF,"Na lavej strane sa nachadza nepovoleny identifikator '%s'.\n", lexema->lex);
        if(retType != node->data->value->type)                                     // AK NIE JE TYP LAVEJ STRANY ZHODNY S TYPOM PRAVEJ TAK CHYBA
        error(ERR_SEM_TYPE,"Typ pravej strany je iny nez typ lavej strany '%s'.", lexema->lex);   
    }
    node->data->flags = (LEX_FLAGS_INIT | node->data->flags);                       // LAVA STRANA JE V KAZDOM PRIPADE INICIALIZOVANA
    
    if(node != pointers->CURRENTFUNCT){                                             //  AK NIE JE NA LAVEJ STRANE SUCASNA FUNKCIA TAK SA JEDNA O PRIRADENIE PRIAMO DO STROMU
        SEM_generate(OP_POP, NULL, NULL, pointers->ACCREG);
        SEM_generate(OP_ASSIGN, pointers->ACCREG, NULL, node->data->value);
    }                                                                               // INAK OSTAVA VYSLEDOK FUNKCIE NA VRCHOLE ZASOBNIKA

    return;
}


//!< FUNKCIE PRE IF-THEN-ELSE STATEMENT
void SEM_thenStat(){
    ETermType condition = SEM_popSS(pointers->EXPRSTACK);                               //  VYSLEDOK PODMIENKY JE NA VRCHOLE ZASOBNIKA
    if(condition != TERM_BOOL)                                                          //  ERROR AK NIE JE BOOL
        error(ERR_SEM_TYPE,"Podmienka v IF nie je typu boolean\n");
        
    SEM_generate(OP_POP, NULL, NULL, pointers->SREG1);                      //  VYGENEROVANIE INSTRUKCII NA VYHODNOTENIE PODMIENKY
    SEM_generate(OP_NOT, pointers->SREG1, NULL, pointers->ACCREG);
    
    TTerm * labelElse = malloc(sizeof(struct STerm));                       //  VYTVORENIE LABELOV PRE VETVU ELSE A PRE KONIEC IFU
    if(labelElse == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    TTerm * labelEnd  = malloc(sizeof(struct STerm));
    if(labelEnd == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    
    labelElse->type = TERM_EIP;
    labelEnd->type = TERM_EIP;
    
    SEM_pushLS(pointers->LABELSTACK, labelElse);                            //  NA VRCHOLE LABEL KONCA A POD NIM LABEL ELSE
    SEM_addCL(pointers->CONSTLIST, labelElse);
    SEM_pushLS(pointers->LABELSTACK, labelEnd);
    SEM_addCL(pointers->CONSTLIST, labelEnd);
    SEM_generate(OP_JTRUE, pointers->ACCREG, labelElse, NULL);              //  AK JE PODMIENKA FALSE, TAK SA SKACE NA LABEL ELSE           
}


void SEM_elseStat(){
    TTerm * labelEnd = SEM_popLS(pointers->LABELSTACK);                     //  NA VRCHOLE JE LABEL END
    SEM_generate(OP_JMP, labelEnd, NULL, NULL);                             //  VYGENEROVANIE DUMMY SKOKU NA KONIEC IFU AK JE PODMIENKA TRUE
    TTerm * labelElse = SEM_popLS(pointers->LABELSTACK);                    //  NA VRCHOLE JE LABEL ELSE
    labelElse->value.address = pointers->PROGRAMINDEX;                      //  NASTAVENIE JEHO ADRESY, UKAZUJE NA DALSIU INSTRUKCIU
    SEM_pushLS(pointers->LABELSTACK, labelEnd);                             //  NAVRAT LABELU KONCA NA VRCHOL ZASOBNIKA
}


void SEM_endIf(){
    TTerm * labelEnd = SEM_popLS(pointers->LABELSTACK);                     //  NA VRCHOLE JE LABEL KONCA
    labelEnd->value.address = pointers->PROGRAMINDEX;                       //  NAPLNENIE ADRESY, UKAZUJE NA DALSIU INSTRUKCIU
}


//!< FUNKCIE PRE WHILE
void SEM_whileStat(){
    TTerm * labelWhile = malloc(sizeof(struct STerm));                      //  VYTVORENIE NAVESTIA SKOKU Z KONCA WHILE (PRED VYHODNOTENIM PODMIENKY)
    if(labelWhile == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    labelWhile->type = TERM_EIP;
    labelWhile->value.address = pointers->PROGRAMINDEX;                     //  NASTAVENIE ADRESY
   
    
    SEM_addCL(pointers->CONSTLIST, labelWhile);                             //  PRIDANIE NA VRCHOL ZASOBNIKA A DO ZOZNAMU KONSTANT
    SEM_pushLS(pointers->LABELSTACK, labelWhile);
}


void SEM_whileBegin(){
    ETermType condition = SEM_popSS(pointers->EXPRSTACK);                   //  NA VRCHOLE ZASOBNIKA JE VYSLEDOK VYHODNOTENIA PODMIENKY
    if(condition != TERM_BOOL)                                              //  AK NIE JE BOOL TAK CHYBA PODMIENKY
        error(ERR_SEM_TYPE,"Podmienka v cykle WHILE nie je typu boolean\n");
    
    SEM_generate(OP_POP, NULL, NULL, pointers->SREG1);                      //  VYGENEROVANIE INSTRUKCII KTORE VYHODNOTIA PODMIENKU
    SEM_generate(OP_NOT, pointers->SREG1, NULL, pointers->ACCREG);    
    
    TTerm * labelEnd = malloc(sizeof(struct STerm));                        //  VYTVORENIE LABELU SKOKU AK JE PODMIENKA FALSE
    if(labelEnd == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    labelEnd->type = TERM_EIP;
    
    SEM_pushLS(pointers->LABELSTACK, labelEnd);
    SEM_addCL(pointers->CONSTLIST, labelEnd);
    
    SEM_generate(OP_JTRUE, pointers->ACCREG, labelEnd, NULL);               //  SKOK NA KONIEC WHILE AK JE PODMIENKA FALSE
}


void SEM_whileEnd(){
    TTerm * labelEnd = SEM_popLS(pointers->LABELSTACK);                     //  LABEL ENDU JE NA VRCHOLE ZASOBNIKA
    TTerm * labelWhile = SEM_popLS(pointers->LABELSTACK);                   //  LABEL ZACIATKU JE POD NIM
    
    SEM_generate(OP_JMP, labelWhile, NULL,NULL);                            //  DUMMY SKOK NA ZACIATOK WHILE
    labelEnd->value.address = pointers->PROGRAMINDEX;                       //  NAPLNENIE NAVESTIA ENDU
}


//!< DALSIE FUNKCIE
void SEM_insertEmbFunc(){
    PTStructLex forward = malloc(sizeof( TStructLex));
        if(forward == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    forward->lex = "forward";
    forward->type = KEY_FORWARD;
    
    PTStructLex strednik = malloc(sizeof( TStructLex));
        if(strednik == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    strednik->lex = ";";
    strednik->type = STREDNIK;
    
    PTStructLex fType = malloc(sizeof( TStructLex));
        if(fType == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pType1 = malloc(sizeof( TStructLex));
        if(pType1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pType2 = malloc(sizeof( TStructLex));
        if(pType2 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pType3 = malloc(sizeof( TStructLex));
        if(pType3 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    
    // Funkcia length
    PTStructLex fLength = malloc(sizeof( TStructLex));
    if(fLength == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pLength1 = malloc(sizeof( TStructLex));
    if(pLength1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");

        
        fLength->value = &EMBlength;
        fLength->lex = "length";    
        fLength->param = NULL;
        fLength->value = NULL;
        fLength->flags = LEX_FLAGS_INIT;
        pLength1->lex = "s";        
        pLength1->type = IDENTIFICATOR;
        pLength1->param = NULL;
        pLength1->value = NULL;
        pType1->lex = "string";
        pType1->type = KEY_STRING;
        fType->lex = "integer";
        fType->type = KEY_INTEGER;

    SEM_defineFunction(fLength);   
    SEM_defineParam(pLength1, pType1);
    SEM_defFuntionType(fType);
    SEM_endFunctionDef(forward);    // FUNKCIA JE REDEKLAROVATELNA
    
    
    // FUNKCIA COPY   
    PTStructLex fCopy = malloc(sizeof( TStructLex));
        if(fCopy == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pCopy1 = malloc(sizeof( TStructLex));
        if(pCopy1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pCopy2 = malloc(sizeof( TStructLex));
        if(pCopy2 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pCopy3 = malloc(sizeof( TStructLex));
        if(pCopy3 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    
        
        fCopy->value = &EMBcopy;
        fCopy->lex = "copy";      
        fCopy->param = NULL;
        fCopy->value = NULL;   
        fCopy->flags = LEX_FLAGS_INIT;        
        pCopy1->lex = "s";     
        pCopy1->type = IDENTIFICATOR; 
        pCopy1->param = NULL;
        pCopy1->value = NULL;        
        pType1->lex = "string";
        pType1->type = KEY_STRING;       
        pCopy2->lex = "i";        
        pCopy2->type = IDENTIFICATOR;      
        pCopy2->param = NULL;
        pCopy2->value = NULL;        
        pType2->lex = "string";
        pType2->type = KEY_INTEGER;
        pCopy3->lex = "n";        
        pCopy3->type = IDENTIFICATOR;
        pCopy3->param = NULL;
        pCopy3->value = NULL;        
        pType3->lex = "string";
        pType3->type = KEY_INTEGER;        
        fType->lex = "integer";
        fType->type = KEY_INTEGER;
    
    SEM_defineFunction(fCopy); 
    SEM_defineParam(pCopy1, pType1);
    SEM_defineParam(pCopy2, pType2);
    SEM_defineParam(pCopy3, pType3);
    SEM_defFuntionType(fType);
    SEM_endFunctionDef(forward);   // FUNKCIA JE REDEKLAROVATELNA
   
    // FUNKCIA FIND
    PTStructLex fFind = malloc(sizeof( TStructLex));
    if(fFind == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pFind1 = malloc(sizeof( TStructLex));
    if(pFind1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pFind2 = malloc(sizeof( TStructLex));
    if(pFind2 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");

 
        fFind->value = &EMBfind;
        fFind->lex = "find";  
        fFind->param = NULL;
        fFind->value = NULL;  
        fFind->flags = LEX_FLAGS_INIT;         
        pFind1->lex = "s";        
        pFind1->type = IDENTIFICATOR;  
        pFind1->param = NULL;
        pFind1->value = NULL;        
        pType1->lex = "string";
        pType1->type = KEY_STRING;       
        pFind2->lex = "search";        
        pFind2->type = IDENTIFICATOR; 
        pFind2->param = NULL;
        pFind2->value = NULL;        
        pType2->lex = "string";
        pType2->type = KEY_STRING;
        fType->lex = "integer";
        fType->type = KEY_INTEGER;
 
    SEM_defineFunction(fFind);
    SEM_defineParam(pFind1, pType1);    
    SEM_defineParam(pFind2, pType2);       
    SEM_defFuntionType(fType);
    SEM_endFunctionDef(strednik);       // FUNKCIU NIE JE MOZNE REDEKLAROVAT
    
    // FUNCKIA SORT
    PTStructLex fSort = malloc(sizeof(TStructLex));
        if(fSort == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pSort1 = malloc(sizeof(TStructLex));
        if(pSort1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");   
    
        fSort->value = &EMBsort;
        fSort->lex = "sort";
        fSort->param = NULL;
        fSort->value = NULL;
        fSort->flags = LEX_FLAGS_INIT;        
        pSort1->lex = "s";        
        pSort1->type = IDENTIFICATOR;
        pSort1->param = NULL;
        pSort1->value = NULL;          
        pType1->lex = "string";
        pType1->type = KEY_STRING;       
        fType->lex = "string";
        fType->type = KEY_STRING;
        
    SEM_defineFunction(fSort);
    SEM_defineParam(pSort1, pType1);       
    SEM_defFuntionType(fType);
    SEM_endFunctionDef(strednik);    
   
    
    free(fType);
    free(pType1);
    free(pType2);
    free(pType3);
    free(forward);
    free(strednik);
}


// FUNKCIE KTORE NASTAVIA ZACIATOK PROGRAMU
void SEM_prologue(){
    TTerm * startLabel = malloc(sizeof(struct STerm));
    if(startLabel == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    startLabel->type = TERM_EIP;
    SEM_pushLS(pointers->LABELSTACK, startLabel);
    SEM_addCL(pointers->CONSTLIST, startLabel);
    
    SEM_generate(OP_JMP, startLabel, NULL,NULL);
}


void SEM_mainBegin(){
    TTerm * startLabel = SEM_popLS(pointers->LABELSTACK);
    startLabel->value.address = pointers->PROGRAMINDEX;
}


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



