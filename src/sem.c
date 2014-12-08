#include "inc.h"

//!<  GLOBALNE PREMENNE
     
extern PGLOB_DEST pointers;
   
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
        if (toDelete->constTerm != NULL && toDelete->constTerm->type == TERM_STRING)
            free(toDelete->constTerm->value.string);
        free(toDelete->constTerm);
        free(toDelete);
    }
    free(list);
}


//!<  FUNKCIE POTREBNE PRI DEFINICII A DEKLARACII PREMENNYCH A FUNKCII
void SEM_defineGlobal(PTStructLex dataID, PTStructLex dataType){
    TSbinstrom newNode = BS_Find(pointers->SYM_TABLE, dataID);                                          //  HLADA PREMENNU V TABULKE SYMBOLOV
    if(newNode != NULL) error(ERR_SEM_UNDEF,"Identifikator '%s' uz bol definovany\n", dataID->lex);     //  AK NAJDE TAK JE INVALIDNY IDENTIFIKATOR
    newNode = BS_Add(pointers->SYM_TABLE, dataID);                                                      //  INAK NENAJDE, A PRIDAVA DO TABULKY SYMBOLOV
    newNode->data->param = NULL;
    newNode->data->value = malloc(sizeof(struct STerm));                                    //  ALOKACIA TERMU GLOBALNEJ PREMENNEJ
    if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    newNode->data->value->name = dataID->lex;
    switch(dataType->type){                                                                 //  PODLA DATOVEHO TYPU NASTAVUJE TYP GLOBALNEJ PREMENNEJ
        case KEY_INTEGER: newNode->data->value->type = TERM_INT;       break;        
        case KEY_STRING:  newNode->data->value->type = TERM_STRING;    break;    
        case KEY_REAL:    newNode->data->value->type = TERM_REAL;      break;    
        case KEY_BOOLEAN: newNode->data->value->type = TERM_BOOL;      break;    
        default : break;
    }

    newNode->data->value->index = false;                                                     // GLOBALNA PREMENNA NIE JE INDEXOM DO ZASOBNIKA
}


void SEM_defineFunction(PTStructLex dataID){
    TSbinstrom newNode = BS_Find(pointers->SYM_TABLE, dataID);          //  PRI DEFINICII FUNKCIE SA HLADA V GLOBALNEJ TABULKE SYMBOLOV

    if(newNode != NULL){                                                //  AK SA NASIEL UZOL S ROVNAKYM ID AKO JE HLADANY(VKLADANY)
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNCTION) == 0)       //  AK NAJDENY UZOL NIE JE FUNKCIA -> ERROR, ID FUNKCIE = GLOBALNEJ PREMENNEJ
            error(ERR_SEM_UNDEF,"Chyba pri definicii funkcie '%s'. Existuje premenna s rovnakym nazvom.\n", dataID->lex);
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNC_DEF) != 0)       //  AK JE NAJDENY UZOL FUNKCIA, KTORA UZ BOLA DEKLAROVANA -> ERROR, REDEFINICIA FUNKCIE
            error(ERR_SEM_UNDEF,"Pokus o redefiniciu funkcie '%s'.\n", dataID->lex);
        free(dataID->lex);
        free(dataID);
    }
    else {                                                              //  INAK SA VKLADA NOVA FUNKCIA
        newNode = BS_Add(pointers->SYM_TABLE,dataID);                   //  PRIDAVA SA NA GLOBALNU UROVEN      
        newNode->data->param = malloc(32);                              //  PRIPRAVA STRINGU PARAMETROV
        if(newNode->data->param == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");   
        newNode->data->value = malloc(sizeof(struct STerm));            //  PRIPRAVA TERMU, VOLANEJ ADRESY PRI VOLANI FUNKCIE
        if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");
        newNode->data->value->type = TERM_EIP;                          //  NASTAVENIE TYPU ADRESY
        newNode->data->flags = LEX_FLAGS_TYPE_FUNCTION;                 //  OZNACENIE ZE SA JEDNA O FUNKCIU
        newNode->data->value->name = dataID->lex;
        newNode->data->value->index = false;
        /*newNode->data->value->value = dataID->value->value;*/   //!< ZMENA TU
    }
    pointers->CURRENTFUNCT = newNode;               //  NASTAVENIE SUCASNEJ FUNKCIE
    pointers->SCOPE = newNode->loc_table;           //  NASTAVENIE JEJ PODSTROMU
    
    pointers->PARAMCOUNT = 0;                            //  VYNULOVANIE POCITADLA
    return;
}


void SEM_defineParam(PTStructLex dataID, PTStructLex dataType){
    TSbinstrom funcParam;
    if( (pointers->CURRENTFUNCT->data->flags & LEX_FLAGS_TYPE_FUNC_DEK) != 0 ){                    // AK SA JEDNA O UZ DEKLAROVANU FUNKCIU, TAK SA KONA LEN TYPOVA KONTROLA
        funcParam = BS_Find(pointers->SCOPE, dataID);                                              // TAK SA HLADA PRVOK V LOKALNEJ TABULKE 
        if(funcParam == NULL) error(ERR_SEM_TYPE,"Parametre v deklaracii a definicii funkcie sa nezhoduju\n");   //  AK SA PARAMETER S DANYM ID NENASIEL = CHYBA
        int i = 0;
        for (char *s = pointers->CURRENTFUNCT->data->param; *s != 'x' && *s != 0; s++)
            i++;

        if(funcParam->data->value->value.offset != pointers->PARAMCOUNT-i-1)                       
            error(ERR_SEM_TYPE,"Parametre v deklaracii a definicii funkcie sa nezhoduju (chybna pozicia parametra '%s')\n", dataID->lex); // AK SA PARAMETER NASIEL, ALE NESEDI JEHO POZICIA = CHYBA
#if 0
        funcParam->data->value->name = dataID->lex;
#else
        free(dataID->lex);
#endif
        switch(dataType->type){                                                                 // AK SA NASIEL A SEDI JEHO POZICIA, ALE NESEDI TYP = CHYBA
            case KEY_INTEGER: if(funcParam->data->value->type != TERM_INT) 
                                error(ERR_SEM_TYPE,"Chybne parametre pri deklaracii funkcie (chybny typ parametra '%s')\n", dataID->lex);    break;        
            case KEY_STRING:  if(funcParam->data->value->type != TERM_STRING) 
                                error(ERR_SEM_TYPE,"Chybne parametre pri deklaracii funkcie (chybny typ parametra '%s')\n", dataID->lex);    break;    
            case KEY_REAL:    if(funcParam->data->value->type != TERM_REAL) 
                                error(ERR_SEM_TYPE,"Chybne parametre pri deklaracii funkcie (chybny typ parametra '%s')\n", dataID->lex);    break;    
            case KEY_BOOLEAN: if(funcParam->data->value->type != TERM_BOOL) 
                                error(ERR_SEM_TYPE,"Chybne parametre pri deklaracii funkcie (chybny typ parametra '%s')\n", dataID->lex);    break;    
            default : break;
        }
        free(dataID);
        (pointers->PARAMCOUNT)++;
        return; // INAK OK
    }
    else{    // AK SA JEDNA O NEDEKLAROVANU FUNKCIU
        if(strcmp(dataID->lex,pointers->CURRENTFUNCT->data->lex) == 0)                      
            error(ERR_SEM_TYPE,"Parameter funkcie nemoze mat rovnaky nazov ako funkcia sama\n");
            
        funcParam = BS_Find(pointers->SCOPE, dataID);                         // PRIDANIE NOVEHO PRVKU DO PODSTROMU FUNKCIE
        if (funcParam != NULL) error(ERR_SEM_UNDEF,"Identifikator parametra '%s' je pouzity 2 krat\n", dataID->lex);
        funcParam = BS_Add(pointers->SCOPE, dataID);
        funcParam->data->value = malloc(sizeof(struct STerm));
        if(funcParam->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
        
        switch(dataType->type){                                                         // NASTAVENIE DATOVEHO TYPU  A ROZSIRENIE STRINGU FUNKCIE
            case KEY_INTEGER: funcParam->data->value->type = TERM_INT;    
                              LEX_string(&(pointers->CURRENTFUNCT->data->param),'i',&(pointers->PARAMCOUNT));   break;         // lexstring zvysuje paramcount  
            case KEY_STRING:  funcParam->data->value->type = TERM_STRING; 
                              LEX_string(&(pointers->CURRENTFUNCT->data->param),'s',&(pointers->PARAMCOUNT));   break;    
            case KEY_REAL:    funcParam->data->value->type = TERM_REAL;   
                              LEX_string(&(pointers->CURRENTFUNCT->data->param),'r',&(pointers->PARAMCOUNT));   break;    
            case KEY_BOOLEAN: funcParam->data->value->type = TERM_BOOL;   
                              LEX_string(&(pointers->CURRENTFUNCT->data->param),'b',&(pointers->PARAMCOUNT));   break;    
            default : break;
        } 
        funcParam->data->value->index = true;                                           // PARAMETER JE INDEXOVY UKAZATEL DO ZASOBNIKA
        SEM_pushLS(pointers->LABELSTACK, funcParam->data->value);
        funcParam->data->value->init = false; 
        return;       
    }
}


void SEM_defFuntionType(PTStructLex dataType){  
    uint32_t flagy = pointers->CURRENTFUNCT->data->flags;       //  FLAGY SUCASNEJ FUNKCIE
    
    if((flagy & LEX_FLAGS_TYPE_FUNC_DEK) != 0){                 //  AK JE DEKLAROVANA, TAK SA KONTROLUJE NAVRATOVY TYP
        switch(dataType->type){
            case KEY_INTEGER: if((flagy & LEX_FLAGS_TYPE_INT) == 0)    error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ funkcie pri definicii\n");  break;        
            case KEY_STRING:  if((flagy & LEX_FLAGS_TYPE_STRING) == 0) error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ funkcie pri definicii\n");  break;    
            case KEY_REAL:    if((flagy & LEX_FLAGS_TYPE_REAL) == 0)   error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ funkcie pri definicii\n");  break;    
            case KEY_BOOLEAN: if((flagy & LEX_FLAGS_TYPE_BOOL) == 0)   error(ERR_SEM_TYPE,"Semanticka chyba! Nespravny navratovy typ funkcie pri definicii\n");  break;    
            default : break;
            }
            pointers->CURRENTFUNCT->data->value->value.address = pointers->PROGRAMINDEX;
            return;
    }    

    switch(dataType->type){                                     //  INAK SA FUNKCIA DEFINUJE (OPATOVNU DEKLARACIU ODCHYTI SEMANTIKA NESKOR AKO CHYBU)
        case KEY_INTEGER: pointers->CURRENTFUNCT->data->flags = (flagy | LEX_FLAGS_TYPE_INT);      break;        
        case KEY_STRING:  pointers->CURRENTFUNCT->data->flags = (flagy | LEX_FLAGS_TYPE_STRING);   break;    
        case KEY_REAL:    pointers->CURRENTFUNCT->data->flags = (flagy | LEX_FLAGS_TYPE_REAL);     break;   
        case KEY_BOOLEAN: pointers->CURRENTFUNCT->data->flags = (flagy | LEX_FLAGS_TYPE_BOOL);     break;   
        default : break;
    }
    
    TTerm * param;
    int i = -2;
    int k = strlen(pointers->CURRENTFUNCT->data->param);
    while(k != 0){
        param = SEM_popLS(pointers->LABELSTACK);
        param->value.offset = i;
        i--;
        k--;
    }    
    
    pointers->CURRENTFUNCT->data->value->value.address = pointers->PROGRAMINDEX;    //  NASTAVENIE ADRESY SKOKU NA NASLEDUJUCU INSTRUKCIU
    return;
}


void SEM_defineLocal(PTStructLex dataID, PTStructLex dataType){
    if(strcmp(dataID->lex,pointers->CURRENTFUNCT->data->lex) == 0)                  //  LOKALNA PREMENNA SA MOZE VOLAT HOCIAKO LEN NIE AKO FUNKCIA
        error(ERR_SEM_UNDEF,"Lokalna premenna funkcie nemoze mat rovnaky nazov ako funkcia sama\n");

    TSbinstrom newNode = BS_Find(pointers->SCOPE, dataID);                          //  HLADA SA HODNOTA V SCOPE
    if(newNode != NULL) error(ERR_SEM_UNDEF,"Identifikator lokalnej premennej '%s' uz bol definovany\n", dataID->lex);  //  AK NAJDE TAK ERROR
    newNode = BS_Add(pointers->SCOPE, dataID);                                      //  AK NENAJDE, TAK SA PRIDAVA (GLOBALNE SA ZATIENUJU)
    
    newNode->data->param = NULL;
    newNode->data->value = malloc(sizeof(struct STerm));                            //  PRIPRAVA TERMU
    if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    
    switch(dataType->type){
        case KEY_INTEGER: newNode->data->value->type = TERM_INT;     LEX_string(&(pointers->CURRENTFUNCT->data->param),'x',&(pointers->PARAMCOUNT));  break;       // ZVYSENIE PARAMCOUNT   
        case KEY_STRING:  newNode->data->value->type = TERM_STRING;  LEX_string(&(pointers->CURRENTFUNCT->data->param),'x',&(pointers->PARAMCOUNT));  break;    
        case KEY_REAL:    newNode->data->value->type = TERM_REAL;    LEX_string(&(pointers->CURRENTFUNCT->data->param),'x',&(pointers->PARAMCOUNT));  break;        
        case KEY_BOOLEAN: newNode->data->value->type = TERM_BOOL;    LEX_string(&(pointers->CURRENTFUNCT->data->param),'x',&(pointers->PARAMCOUNT));  break;    
        default : break;
    }
    int i = 0;
    for (char *pom = pointers->CURRENTFUNCT->data->param; *pom != 'x'; pom++)
        i++;
    newNode->data->value->index = true;                                                 
    newNode->data->value->value.offset = (pointers->PARAMCOUNT) - i;    // LOKALNE PREMENNE MAJU INDEX VYSSI O 2, PRETOZE SA MEDZI NIMY A LOKALNYMI BUDE NACHADZAT ADRESA SPATNEHO SKOKU
    newNode->data->value->init = false;
    newNode->data->value->name = dataID->lex;
    SEM_generate(OP_PUSH, pointers->SREG1, NULL, NULL);                 //  PUSH LOKALNEJ NA MIROV ZASOBNIK
    return;
}


void SEM_endFunctionDef(PTStructLex lexema){
    
    PTStructLex data = pointers->CURRENTFUNCT->data;
    
    if((lexema->type == KEY_FORWARD)&&((data->flags & LEX_FLAGS_TYPE_FUNC_DEK) != 0)) //  AK SA DEFINICIA FUNKCIE KONCI FORWARDOM A FUNKCIA JE DEKLAROVANA TAK OPATOVNA REDEKLARACIA
        error(ERR_SEM_UNDEF,"Opatovna redeklaracia funkcie '%s'\n", data->lex);

     
    data->flags = (data->flags | LEX_FLAGS_TYPE_FUNC_DEK);  // FUNKCIA SA OZNACI AKO DEKLAROVANA
    
    if((lexema->type == STREDNIK)&&(strcmp(data->lex, "find")!=0)&&(strcmp(data->lex, "sort")!=0)){                                   //  AK SA UKONCUJE STREDNIKOM, TEDA SA FUNKCIA DEFINOVALA                                 
        unsigned int dlzka = strlen(data->param);    //  ULOZENIE POCTU LOKALNYCH + PARAMETROV
        char * x = strstr(data->param, "x");                          //  ADRESA PRVEHO PRVKU X    
  
        TTerm * pocetParametrov = malloc(sizeof(struct STerm));
        if (pocetParametrov == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
        pocetParametrov->value.integer = (x == NULL)?((int)dlzka):(int)(x - data->param);      
        // POCET PARAMETROV, JE DLZKA STRINGU PARAM AK NEOBSAHUJE ZIADNE X, INAK JE TO ADRESA PRVEHO X MINUS ADRESA PRVEHO PRVKU
        pocetParametrov->index = false;
        TTerm * pocetLokalnych = malloc(sizeof(struct STerm));
        if (pocetLokalnych == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
        pocetLokalnych->value.integer = ((x == NULL)?(0):(dlzka - pocetParametrov->value.integer));
        pocetLokalnych->index = false;
        pocetLokalnych->type = TERM_INT;
        pocetParametrov->type = TERM_INT;
        //  POCET LOKALNYCH, JE 0 AK NENASLO X, INAK JE TO DLZKA STRINGU MINUS POCET PARAMETROV
        
        SEM_addCL(pointers->CONSTLIST, pocetLokalnych);
        SEM_addCL(pointers->CONSTLIST, pocetParametrov);
        SEM_generate(OP_RET, pocetLokalnych, pocetParametrov, NULL);
        
        data->flags = (data->flags | LEX_FLAGS_TYPE_FUNC_DEF);  // OZNACENIE FUNKCIE AKO DEFINOVANEJ
    }
    
    if ((strcmp(data->lex, "find")==0)||(strcmp(data->lex, "sort")==0))
        data->flags = (data->flags | LEX_FLAGS_TYPE_FUNC_DEF);
        
    pointers->SCOPE = pointers->SYM_TABLE;  // SCOPE SA VRACIA NA SYMTABLE
    pointers->CURRENTFUNCT = NULL;          // NENACHADZA SA V ZIADNEJ FUNKCII
}

void SEM_checkFunction(PTStructLex lexema){
    if((lexema->flags & LEX_FLAGS_TYPE_FUNCTION)!=0)
        if((strcmp(lexema->lex, "copy") != 0)&&(strcmp(lexema->lex, "length") != 0)&&((lexema->flags & LEX_FLAGS_TYPE_FUNC_DEF)==0)) // AK NIE JE COPY, NIE JE LENGTH A NIE JE DEFINOVANA
            error(ERR_SEM_UNDEF,"Funkcia '%s' nie je definovana.\n", lexema->lex);
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
           error(ERR_SEM_UNDEF,"Nepovolene pouzitie identifikatora funkcie '%s'.\n", lexema->lex);
        
        if (node == pointers->CURRENTFUNCT)
           error(ERR_SEM_UNDEF,"Nepovolene pouzitie identifikatora funkcie '%s'.\n", lexema->lex);
        
        //if ((node->data->flags & LEX_FLAGS_INIT) == 0)                           // CHYBOVA HLASKA, VOLA SA NEINICIALIZOVANA PREMENNA
        //   error(ERR_SEM_UNDEF,"Neinicializovana hodnota '%s'.\n", lexema->lex);        
        
        SEM_pushSS(pointers->EXPRSTACK, node->data->value->type);        // ULOZENIE TYPU
        //log ("Type of %s is %d\n", node->data->lex, node->data->type);
        if(node->data->value->index){
            SEM_generate(OP_LOAD, node->data->value, NULL, pointers->SREG1);
            SEM_generate(OP_PUSH, pointers->SREG1, NULL, NULL);
        }
        else SEM_generate(OP_PUSH, node->data->value, NULL, NULL);
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
        term->init = true;
        term->index = false;
        SEM_pushSS(pointers->EXPRSTACK, term->type);     // ULOZENIE TYPU NA SEMANTICKY ZASOBNIK
        term->name = lexema->lex;
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
    pointers->ACCREG->index = false;
    pointers->ACCREG->init = true;
    pointers->SREG1->index = false;
    pointers->SREG1->type  = typeRight;
    pointers->SREG2->index = false;
    pointers->SREG2->type  = typeRight;
    SEM_generate(OP_POP, NULL, NULL, pointers->SREG2);
    SEM_generate(OP_POP, NULL, NULL, pointers->SREG1);
    switch(lexema->type){
        case OPERATOR_PLUS     : SEM_generate(OP_PLUS, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);   break;                                                                                 
        case OPERATOR_MINUS    : if(typeRight != TERM_STRING)
                                    SEM_generate(OP_MINUS, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);
                                 else 
                                    error(ERR_SEM_TYPE, "Retazce nie je mozne odcitat");
                                 break;       
        case OPERATOR_TIMES    : if(typeRight != TERM_STRING)
                                    SEM_generate(OP_MUL, pointers->SREG1, pointers->SREG2,  pointers->ACCREG);
                                 else 
                                    error(ERR_SEM_TYPE, "Retazce nie je mozne nasobit");
                                 break;
        case OPERATOR_DIV      : if(typeRight != TERM_STRING)
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
    if(fNode == NULL) error(ERR_SEM_UNDEF,"Volanie nedeklarovanej funkcie '%s'.\n", functID->lex);
    
    if(((fNode->data->flags | LEX_FLAGS_TYPE_FUNC_DEF) == 0)&&(strcmp(functID->lex,"length") != 0)&&(strcmp(functID->lex,"copy") != 0))
        error(ERR_SEM_UNDEF, "Volanie nedefinovanej funkcie '%s'.\n", functID->lex);
    
    pointers->ACCREG->init = false;
    pointers->ACCREG->index = false;
    SEM_generate(OP_PUSH, pointers->ACCREG, NULL, NULL);  // PRIPRAVA HODNOTY VYSLEDKU
    pointers->PARAMCOUNT = 0;            //  VYNULOVANIE POCITADLA PARAMETROV
}


void SEM_functionParam(PTStructLex functID, PTStructLex paramID){
    TSbinstrom fNode;
    if(functID != NULL){ // V PRIPADE VOLANIA WRITE SA NEHLADA NIC V TABULKE SYMBOLOV
        fNode = BS_Find(pointers->SYM_TABLE, functID);                   //  FUNKCIA SA NAJDE V TABULKE SYMBOLOV
        if( ((fNode->data->param)[pointers->PARAMCOUNT] == 'x') || ((int)strlen(fNode->data->param)<pointers->PARAMCOUNT+1) )
            error(ERR_SEM_TYPE,"Prilis vela parametrov pri volani funkcie '%s'.\n", functID->lex);
    }
    if(paramID->type == IDENTIFICATOR){                                         //  AK SA NASIEL IDENTIFIKATOR
        TSbinstrom pNode = BS_Find(pointers->SCOPE, paramID);                       //  PARAMETER SA HLADA V SCOPE
        if((pNode == NULL)&&(pointers->SYM_TABLE != pointers->SCOPE))               //  AK NIE JE V SCOPE TAK V TABULKE SYMBOLOV
            pNode = BS_Find(pointers->SYM_TABLE, paramID);
        
        if(pNode == NULL) error(ERR_SEM_UNDEF,"Nedefinovany parameter funkcie '%s'.\n", paramID->lex);  //  AK SA NENAJDE TAK JE NEDEFINOVANY
    
        if((pNode->data->flags & LEX_FLAGS_TYPE_FUNCTION) != 0) 
            error(ERR_SEM_TYPE,"Parametrom funkcie je identifikator funkcie '%s'.\n", paramID->lex);    // AK SA NAJDE A JE TO IDENTIFIKATOR FUNKCIE TAK ERROR
        
        
        if(functID != NULL)      // AK NIE JE WRITE, TAK SA ROBI TYPOVA KONTROLA
        switch(pNode->data->value->type){   // INAK SA ROBI TYPOVA KONTROLA
            case TERM_INT   : if((fNode->data->param)[pointers->PARAMCOUNT] != 'i') 
                                error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT + 1, functID->lex);break;
            case TERM_REAL  : if((fNode->data->param)[pointers->PARAMCOUNT] != 'r') 
                                error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT + 1, functID->lex);break;
            case TERM_STRING: if((fNode->data->param)[pointers->PARAMCOUNT] != 's') 
                                error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT + 1, functID->lex);break;
            case TERM_BOOL  : if((fNode->data->param)[pointers->PARAMCOUNT] != 'b') 
                                error(ERR_SEM_TYPE,"Typ parametra na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT + 1, functID->lex);break;
            default : break;
        }
#if 0
        size_t lexSize = strlen(paramID->lex)+1;
        pNode->data->value->name = malloc(lexSize);
        memcpy(pNode->data->value->name, paramID->lex, lexSize);
#else
        pNode->data->value->name = pNode->data->lex;
#endif
        pointers->SREG1->index = false;
        pointers->SREG1->value.integer = -1;
        if(pNode->data->value->index){
            SEM_generate(OP_LOAD, pNode->data->value, NULL, pointers->SREG1);
            SEM_generate(OP_PUSH, pointers->SREG1, NULL, NULL);
        }
        else {
            SEM_generate(OP_PUSH, pNode->data->value, NULL, NULL);  // PUSH PARAMETRA NA ZASOBNIK
        }
    }
    else{                                                            // V PRIPADE KONSTANTY JU POTREBUJEM ULOZIT DO STROMU AKO GENEROVANU PREMENNU   
        struct STerm * term = malloc(sizeof(struct STerm));          // PRE KONSTANTY SA VYTVARA NOVE UMIESTNENIE, A UKLADA SA DO ZOZNAMU KONSTANT
        if (term == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!.\n");
        term->index = false;
        term->init = true;
        size_t size;
        switch(paramID->type){                                        // PODLA TYPU LEXEMY SA SKONVERTUJE STRING NA POZADOVANU HODNOTU
            case INT_CONST    :  term->value.integer = atoi(paramID->lex);
                                 term->type = TERM_INT; break;
            case REAL_CONST   :  term->value.real = atof(paramID->lex);
                                 term->type = TERM_REAL; break;  
            case STRING_CONST :  size = strlen(paramID->lex)+1;
                                 term->value.string = malloc(size);
                                 memcpy(term->value.string, paramID->lex, size);
                                 term->type = TERM_STRING; break;           
            case KEY_TRUE   :    term->value.boolean = true;
                                 term->type = TERM_BOOL; break;           
            case KEY_FALSE  :    term->value.boolean = false;
                                 term->type = TERM_BOOL; break;
            default : break;
        }
        
        if(functID != NULL)
        switch(term->type){   // INAK SA ROBI TYPOVA KONTROLA
            case TERM_INT   : if((fNode->data->param)[pointers->PARAMCOUNT] != 'i') 
                                error(ERR_SEM_TYPE,"Typ parametra (const) na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT + 1, functID->lex);break;
            case TERM_REAL  : if((fNode->data->param)[pointers->PARAMCOUNT] != 'r') 
                                error(ERR_SEM_TYPE,"Typ parametra (const) na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT + 1, functID->lex);break;
            case TERM_STRING: if((fNode->data->param)[pointers->PARAMCOUNT] != 's') 
                                error(ERR_SEM_TYPE,"Typ parametra (const) na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT + 1, functID->lex);break;
            case TERM_BOOL  : if((fNode->data->param)[pointers->PARAMCOUNT] != 'b') 
                                error(ERR_SEM_TYPE,"Typ parametra (const) na pozicii %d pri volani funkcie '%s' je nespravny.\n", pointers->PARAMCOUNT + 1, functID->lex);break;
            default : break;
        }
#if 0
        size_t lexSize = strlen(paramID->lex)+1;
        term->name = malloc(lexSize);
        memcpy(term->name, paramID->lex, lexSize);
#else
        term->name = NULL;
#endif
        SEM_generate(OP_PUSH, term, NULL, NULL);                             // PUSH PARAMETRA
        SEM_addCL(pointers->CONSTLIST,term);                                 // ULOZENIE KONSTANTY DO ZOZNAMU KONSTANT
    }
    
    (pointers->PARAMCOUNT)++;           // ZVYSENIE POCITADLA PARAMETROV
}


void SEM_functionCall(PTStructLex functID){
    TSbinstrom node = BS_Find(pointers->SYM_TABLE, functID);
    /*if     ((strcmp(functID->lex, "length") == 0))
        SEM_generate(OP_CALL, &EMBlength, NULL, NULL);
    else if((strcmp(functID->lex, "copy") == 0))
        SEM_generate(OP_CALL, &EMBcopy, NULL, NULL);
    else if((strcmp(functID->lex, "sort") == 0))
        SEM_generate(OP_CALL, &EMBsort, NULL, NULL);
    else if((strcmp(functID->lex, "find") == 0))
        SEM_generate(OP_CALL, &EMBfind, NULL, NULL);
    else*/
        SEM_generate(OP_CALL, node->data->value, NULL, NULL);                       //  SKOK NA FUNKCIU 
                       //  SKOK NA FUNKCIU 
    if ( ((node->data->param)[pointers->PARAMCOUNT]=='i')||
         ((node->data->param)[pointers->PARAMCOUNT]=='r')||
         ((node->data->param)[pointers->PARAMCOUNT]=='s')||
         ((node->data->param)[pointers->PARAMCOUNT]=='b') )
        error(ERR_SEM_TYPE,"Nedostatocny pocet parametrov pri volani funkcie '%s'.\n", functID->lex);
    
    
    
    if((node->data->flags & LEX_FLAGS_TYPE_INT) != 0)   { SEM_pushSS(pointers->EXPRSTACK, TERM_INT);  return; }
    if((node->data->flags & LEX_FLAGS_TYPE_REAL) != 0)  { SEM_pushSS(pointers->EXPRSTACK, TERM_REAL); return; }
    if((node->data->flags & LEX_FLAGS_TYPE_BOOL) != 0)  { SEM_pushSS(pointers->EXPRSTACK, TERM_BOOL); return; }
    if((node->data->flags & LEX_FLAGS_TYPE_STRING) != 0){ SEM_pushSS(pointers->EXPRSTACK, TERM_STRING); return; }
}


//!< PRIRADENIE
void SEM_assignValue(PTStructLex lexema){
    
    TSbinstrom node = BS_Find(pointers->SCOPE, lexema);                             // V TABULKE SYMBOLOV NAJDE LAVU STRANU VYRAZU 
    if((node == NULL)&&(pointers->SCOPE != pointers->SYM_TABLE))                 
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
    
    SEM_generate(OP_POP, NULL, NULL, pointers->ACCREG);
    if(node != pointers->CURRENTFUNCT)                                             //  AK NIE JE NA LAVEJ STRANE SUCASNA FUNKCIA TAK SA JEDNA O PRIRADENIE PRIAMO DO STROMU
        SEM_generate(OP_ASSIGN, pointers->ACCREG, NULL, node->data->value);
    else {
        TTerm *pom = malloc (sizeof(TTerm));
        if (pom == NULL) error(ERR_INTERNAL, "Malloc hodil NULL");
        int i = 0;
        for (char *str = pointers->CURRENTFUNCT->data->param; *str != '\0' && *str != 'x'; str++)
            i++;
        pom->index = true;
        pom->value.offset = (i+2)*(-1);
        pom->type = TERM_OFFSET;
        SEM_addCL(pointers->CONSTLIST, pom);
        SEM_generate(OP_ASSIGN, pointers->ACCREG, NULL, pom);
    }
   
    return;
}


//!< FUNKCIE PRE IF-THEN(-ELSE) STATEMENT
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
    labelElse->index = false;
    labelEnd->index = false;
    
    SEM_pushLS(pointers->LABELSTACK, labelElse);                            //  NA VRCHOLE LABEL KONCA A POD NIM LABEL ELSE
    SEM_addCL(pointers->CONSTLIST, labelElse);
    SEM_pushLS(pointers->LABELSTACK, labelEnd);
    SEM_addCL(pointers->CONSTLIST, labelEnd);
    SEM_generate(OP_JTRUE, pointers->ACCREG, labelElse, NULL);              //  AK JE PODMIENKA FALSE, TAK SA SKACE NA LABEL ELSE           
}


void SEM_noElse(){
    TTerm *labelEnd = SEM_popLS(pointers->LABELSTACK);
    TTerm *labelElse = SEM_popLS(pointers->LABELSTACK);
    labelElse->value.address = pointers->PROGRAMINDEX;
    labelEnd->value.address = pointers->PROGRAMINDEX;
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
    labelWhile->index = false;
    
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
    labelEnd->index = false;
    
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


//!< FUNKCIE PRE REPEAT-UNTIL
void SEM_repeatStat(){
    TTerm * labelRepeat = malloc(sizeof(struct STerm));
    if(labelRepeat == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    labelRepeat->type = TERM_EIP;
    labelRepeat->value.address = pointers->PROGRAMINDEX;                     //  NASTAVENIE ADRESY
    labelRepeat->index = false;
    
    SEM_addCL(pointers->CONSTLIST, labelRepeat);                             //  PRIDANIE NA VRCHOL ZASOBNIKA A DO ZOZNAMU KONSTANT
    SEM_pushLS(pointers->LABELSTACK, labelRepeat);    
}


void SEM_repeatEnd(){
    ETermType condition = SEM_popSS(pointers->EXPRSTACK);                   //  NA VRCHOLE ZASOBNIKA JE VYSLEDOK VYHODNOTENIA PODMIENKY
    if(condition != TERM_BOOL)                                              //  AK NIE JE BOOL TAK CHYBA PODMIENKY
        error(ERR_SEM_TYPE,"Podmienka v cykle REPEAT-UNTIL nie je typu boolean\n");
    
    TTerm * labelRepeat = SEM_popLS(pointers->LABELSTACK);
    
    SEM_generate(OP_POP, NULL, NULL, pointers->SREG1);                      //  VYGENEROVANIE INSTRUKCII KTORE VYHODNOTIA PODMIENKU
    SEM_generate(OP_NOT, pointers->SREG1, NULL, pointers->ACCREG);   
    SEM_generate(OP_JTRUE, pointers->ACCREG, labelRepeat, NULL);
}


//!< DALSIE FUNKCIE
void SEM_insertEmbFunc(){     
    TSbinstrom functNode;
    TSbinstrom paramNode;
    
    // Funkcia length
    PTStructLex fLength = malloc(sizeof( TStructLex));
    if(fLength == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pLength1 = malloc(sizeof( TStructLex));
    if(pLength1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    
        
        fLength->value = &EMBlength;                // VALUE FUNKCIE JE IMPLICITNE EMBlength 
        fLength->lex = malloc(7);                   // NAZOV V TABULKE SYMBOLOV = LENGTH
        memcpy (fLength->lex, "length", 7);    
        fLength->param = malloc(2);                 // FUNKCIA MA JEDEN PARAMETER TYPU STRING
        memcpy (fLength->param, "s", 2);  
        fLength->flags = LEX_FLAGS_TYPE_FUNCTION | LEX_FLAGS_TYPE_INT | LEX_FLAGS_TYPE_FUNC_DEK;  //  JEDNA SA O FUNKCIU, KTORA JE LEN DEKLAROVANA A JE TYPU INT
        
        pLength1->lex = malloc(2);                  // PARAMETER S NAZVOM = S
        memcpy(pLength1->lex, "s", 2);
        pLength1->type = IDENTIFICATOR;
        pLength1->param = NULL;
        pLength1->value = malloc(sizeof(struct STerm)); // JEHO TERM
        pLength1->value->value.offset = -2;             // OFFSET PRVEHO PARAMETRA = -2
        pLength1->value->type = TERM_STRING;            // PARAMETER TYPU STRING


    functNode = BS_Add(pointers->SYM_TABLE, fLength);
    pointers->CURRENTFUNCT = functNode;
    paramNode = BS_Add(functNode->loc_table, pLength1);

   
    // FUNKCIA COPY   
    PTStructLex fCopy = malloc(sizeof( TStructLex));
        if(fCopy == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pCopy1 = malloc(sizeof( TStructLex));
        if(pCopy1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pCopy2 = malloc(sizeof( TStructLex));
        if(pCopy2 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pCopy3 = malloc(sizeof( TStructLex));
        if(pCopy3 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    
        
        fCopy->value = &EMBlength;                // VALUE FUNKCIE JE IMPLICITNE EMBcopy
        fCopy->lex = malloc(5);                   // NAZOV V TABULKE SYMBOLOV = COPY
        memcpy (fCopy->lex, "copy", 5);    
        fCopy->param = malloc(4);                 // FUNKCIA MA TRI PARAMETRE TYPU STRING INT INT
        memcpy (fCopy->param, "sii", 4);  
        fCopy->flags = LEX_FLAGS_TYPE_FUNCTION | LEX_FLAGS_TYPE_STRING | LEX_FLAGS_TYPE_FUNC_DEK;  //  JEDNA SA O FUNKCIU, KTORA JE LEN DEKLAROVANA A JE TYPU STRING
        
        pCopy1->lex = malloc(2);                  // PARAMETER S NAZVOM = S
        memcpy(pCopy1->lex, "s", 2);
        pCopy1->type = IDENTIFICATOR;
        pCopy1->param = NULL;
        pCopy1->value = malloc(sizeof(struct STerm)); // JEHO TERM
        pCopy1->value->value.offset = -4;             // OFFSET TRETIEHO PARAMETRA = -4
        pCopy1->value->type = TERM_STRING;            // PARAMETER TYPU STRING

        pCopy2->lex = malloc(2);                  // PARAMETER S NAZVOM = I
        memcpy(pCopy2->lex, "i", 2);
        pCopy2->type = IDENTIFICATOR;
        pCopy2->param = NULL;
        pCopy2->value = malloc(sizeof(struct STerm)); // JEHO TERM
        pCopy2->value->value.offset = -3;             // OFFSET DRUHEHO PARAMETRA = -3
        pCopy2->value->type = TERM_INT;            // PARAMETER TYPU INT
        
        pCopy3->lex = malloc(2);                  // PARAMETER S NAZVOM = N
        memcpy(pCopy3->lex, "n", 2);
        pCopy3->type = IDENTIFICATOR;
        pCopy3->param = NULL;
        pCopy3->value = malloc(sizeof(struct STerm)); // JEHO TERM
        pCopy3->value->value.offset = -2;             // OFFSET PRVEHO PARAMETRA = -2
        pCopy3->value->type = TERM_INT;            // PARAMETER TYPU INT

    functNode = BS_Add(pointers->SYM_TABLE, fCopy);
    pointers->CURRENTFUNCT = functNode;
    paramNode = BS_Add(functNode->loc_table, pCopy1);
    paramNode = BS_Add(functNode->loc_table, pCopy2);
    paramNode = BS_Add(functNode->loc_table, pCopy3);    

    
    // FUNKCIA FIND
    PTStructLex fFind = malloc(sizeof( TStructLex));
    if(fFind == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pFind1 = malloc(sizeof( TStructLex));
    if(pFind1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pFind2 = malloc(sizeof( TStructLex));
    if(pFind2 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");

        fFind->value = &EMBfind;                // VALUE FUNKCIE JE IMPLICITNE EMBfind 
        fFind->lex = malloc(5);                   // NAZOV V TABULKE SYMBOLOV = FIND
        memcpy (fFind->lex, "find", 5);    
        fFind->param = malloc(3);                 // FUNKCIA MA 2 PARAMETRE TYPU STRING
        memcpy (fFind->param, "ss", 3);  
        fFind->flags = LEX_FLAGS_TYPE_FUNCTION | LEX_FLAGS_TYPE_INT | LEX_FLAGS_TYPE_FUNC_DEF;  //  JEDNA SA O FUNKCIU, KTORA JE DEFINOVANA A JE TYPU INT
        
        pFind1->lex = malloc(2);                  // PARAMETER S NAZVOM = S
        memcpy(pFind1->lex, "s", 2);
        pFind1->type = IDENTIFICATOR;
        pFind1->param = NULL;
        pFind1->value = malloc(sizeof(struct STerm)); // JEHO TERM
        pFind1->value->value.offset = -3;             // OFFSET DRUHEHO PARAMETRA = -3
        pFind1->value->type = TERM_STRING;            // PARAMETER TYPU STRING


        pFind2->lex = malloc(7);                      // PARAMETER S NAZVOM = SEARCH
        memcpy(pFind2->lex, "search", 7);
        pFind2->type = IDENTIFICATOR;
        pFind2->param = NULL;
        pFind2->value = malloc(sizeof(struct STerm)); // JEHO TERM
        pFind2->value->value.offset = -2;             // OFFSET PRVEHO PARAMETRA = -2
        pFind2->value->type = TERM_STRING;            // PARAMETER TYPU STRING
        
    functNode = BS_Add(pointers->SYM_TABLE, fFind);
    pointers->CURRENTFUNCT = functNode;
    paramNode = BS_Add(functNode->loc_table, pFind1);
    paramNode = BS_Add(functNode->loc_table, pFind2);
    
    
    // FUNCKIA SORT
    PTStructLex fSort = malloc(sizeof(TStructLex));
        if(fSort == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");
    PTStructLex pSort1 = malloc(sizeof(TStructLex));
        if(pSort1 == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate\n");   
        
        
        fSort->value = &EMBsort;                // VALUE FUNKCIE JE IMPLICITNE EMBsort 
        fSort->lex = malloc(5);                   // NAZOV V TABULKE SYMBOLOV = SORT
        memcpy (fSort->lex, "sort", 5);    
        fSort->param = malloc(2);                 // FUNKCIA MA JEDEN PARAMETER TYPU STRING
        memcpy (fSort->param, "s", 2);  
        fSort->flags = LEX_FLAGS_TYPE_FUNCTION | LEX_FLAGS_TYPE_STRING | LEX_FLAGS_TYPE_FUNC_DEF;  //  JEDNA SA O FUNKCIU, KTORA JE DEFINOVANA A JE TYPU STRING
        
        pSort1->lex = malloc(2);                  // PARAMETER S NAZVOM = S
        memcpy(pSort1->lex, "s", 2);
        pSort1->type = IDENTIFICATOR;
        pSort1->param = NULL;
        pSort1->value = malloc(sizeof(struct STerm)); // JEHO TERM
        pSort1->value->value.offset = -2;             // OFFSET PRVEHO PARAMETRA = -2
        pSort1->value->type = TERM_STRING;            // PARAMETER TYPU STRING


    functNode = BS_Add(pointers->SYM_TABLE, fSort);
    pointers->CURRENTFUNCT = functNode;
    paramNode = BS_Add(functNode->loc_table, pSort1);    
    paramNode->loc_table = NULL;                //!< to len aby nebol warning na unused
}


// FUNKCIE KTORE NASTAVIA ZACIATOK PROGRAMU
void SEM_readln(PTStructLex paramID){
    TSbinstrom pNode = BS_Find(pointers->SCOPE, paramID);                       //  PARAMETER SA HLADA V SCOPE
    if((pNode == NULL)&&(pointers->SYM_TABLE != pointers->SCOPE))               //  AK NIE JE V SCOPE TAK V TABULKE SYMBOLOV
        pNode = BS_Find(pointers->SYM_TABLE, paramID);
    
    if(pNode == NULL)
        error(ERR_SEM_UNDEF,"Nedefinovana premenna v parametri funkcie READLN.\n");
        
    if((pNode->data->flags & LEX_FLAGS_TYPE_FUNCTION) != 0)
        error(ERR_SEM_TYPE,"READLN nemoze nacitat hodnotu do funkcie");
    
    if(pNode->data->value->type == TERM_BOOL)
        error(ERR_SEM_TYPE,"READLN nemoze nacitat boolean.\n");
    
    if(pNode->data->value->index){
        SEM_generate(OP_LOAD, pNode->data->value, NULL, pointers->ACCREG);
        SEM_generate(OP_PUSH, pointers->ACCREG, NULL, NULL);
    }
    else {
        TTerm *PomNode = malloc (sizeof(TTerm));
        PomNode->value.pointer = pNode->data->value;
        PomNode->type = TERM_POINTER;
        SEM_addCL(pointers->CONSTLIST, PomNode);
        SEM_generate(OP_PUSH, PomNode, NULL, NULL);
    }
    
    SEM_generate(OP_CALL, &EMBreadln,NULL,NULL);
    
}

void SEM_writePrologue(){
    pointers->PARAMCOUNT = 0;
}


void SEM_writeCall(){
    TTerm * pCount = malloc(sizeof(struct STerm));
    if(pCount == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate");
    pCount->value.offset = pointers->PARAMCOUNT;
    pCount->init = true;
    pCount->index = false;
    pCount->type = TERM_OFFSET;
    SEM_addCL(pointers->CONSTLIST,pCount);
    SEM_generate(OP_PUSH, pCount, NULL, NULL);
    SEM_generate(OP_CALL, &EMBwrite,NULL,NULL);
    pointers->PARAMCOUNT = 0;
}


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
    startLabel->index = false;
}

static inline void SEM_addInstruction (P3AC inst) {
    uint32_t size = (uint32_t)(PEIP-EIP);
    if ((size+1)%EIP_SIZE == 0) {
        /* 
         * Realokuje EIP o 42 adresu ulozi do EIP a pricita k nej velkost a ulozi do PEIP
         * nasledne od toho cele odcita velkost aby sme vedeli ci nenastal NULL
         */
        PEIP = size + (EIP = realloc(EIP, sizeof(P3AC)*(size+1+EIP_SIZE)));
        if (EIP == NULL)
            error(ERR_INTERNAL, "Chyba realokacie!\n");
    }
    *PEIP = inst;
    PEIP++;
    *PEIP = NULL;
}

void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result){

 // FUNKCIA, KTORA VYTVORI TROJADRESNU INSTRUKCIU A PRIDA JU DO PROGRAMU
  
    P3AC newInstruction = malloc(sizeof(struct S3AC));
    newInstruction->op = operation;
    newInstruction->op1 = op1;
    newInstruction->op2 = op2;
    newInstruction->ret = result;
    (pointers->PROGRAMINDEX)++;
    SEM_addInstruction(newInstruction);
}

