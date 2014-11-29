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

void SEM_typeDefinition(PTStructLex lexema, PTStructLex term_lex){
    TSbinstrom node = BS_Find(pointers->SCOPE, term_lex);
    
    if((pointers->SCOPE != pointers->SYM_TABLE)&&(node != pointers->SCOPE)){    // AK SME VO FUNKCII, A NEPRIRADUJE SA NAVRATOVY TYP FUNKCIE
        if((pointers->SCOPE->data->flags & LEX_FLAGS_TYPE_FUNC_DEF) != 0){      // AK JE FUNKCIA DEFINOVANA TAK POKRACUJE A ROBI TYPOVU KONTROLU
            switch(lexema->type){
            case KEY_INTEGER:
                if(node->data->value->type != TERM_INT) 
                error(ERR_SEM_TYPE,"Semanticka chyba! Nekompatibilne datove typy parametrov funkcie\n");
            break;   
        
            case KEY_STRING:
                if(node->data->value->type != TERM_STRING) 
                error(ERR_SEM_TYPE,"Semanticka chyba! Nekompatibilne datove typy parametrov funkcie\n");
            break;   
        
            case KEY_REAL:
                if(node->data->value->type != TERM_REAL) 
                error(ERR_SEM_TYPE,"Semanticka chyba! Nekompatibilne datove typy parametrov funkcie\n");
            break;   
        
            case KEY_BOOLEAN:
                if(node->data->value->type != TERM_BOOL) 
                error(ERR_SEM_TYPE,"Semanticka chyba! Nekompatibilne datove typy parametrov funkcie\n");
            break;  

            default : break;
            }
            return;
        }
    }
    // AK SA PRIRADUJE TYP GLOBALNEJ PREMENNEJ, FUNKCII ALEBO LOKALNYM PREMENNYM A PARAMETROM NEDEFINOVANEJ FUNKCIE
    switch(lexema->type){
        case KEY_INTEGER:
            node->data->value->type = TERM_INT;
            if(pointers->SCOPE != pointers->SYM_TABLE) LEX_string(&(pointers->SCOPE->data->param),'i',&(pointers->PARAMCOUNT));
        break;
        
        case KEY_STRING:
            node->data->value->type = TERM_STRING;
            if(pointers->SCOPE != pointers->SYM_TABLE) LEX_string(&(pointers->SCOPE->data->param),'r',&(pointers->PARAMCOUNT));
        break;
    
        case KEY_REAL:
            node->data->value->type = TERM_REAL;
            if(pointers->SCOPE != pointers->SYM_TABLE) LEX_string(&(pointers->SCOPE->data->param),'s',&(pointers->PARAMCOUNT));
        break;
    
        case KEY_BOOLEAN:
            node->data->value->type = TERM_BOOL;
            if(pointers->SCOPE != pointers->SYM_TABLE) LEX_string(&(pointers->SCOPE->data->param),'b',&(pointers->PARAMCOUNT));
        break;
    
        default : break;
        }

    return;
}

void SEM_varDec(PTStructLex lexema){
    
    TSbinstrom newNode = BS_Find(pointers->SCOPE, lexema);
    
    if((pointers->SCOPE != pointers->SYM_TABLE)&&(newNode != NULL))         // AK SA HLADA VO FUNKCII A NIECO SA NASLO
        if((pointers->SCOPE->data->flags & LEX_FLAGS_TYPE_FUNC_DEF) != 0){        // AK BOLA FUNKCIA DEFINOVANA (PRIPAD FORWARD)
            if(((unsigned)*(newNode->data->param) == pointers->PARAMCOUNT)&&((pointers->PARAMCOUNT) < strlen(pointers->SCOPE->data->param) ) ) // AK SA INDEX PARAMETRA NAJDENEHO PRVKU ROVNA SUCASNEMU INDEXU A ZAROVEN JE SUCASNY INDEX MENSI NEZ POCET PARAMETROV FUNKCIE
                return;                                                     // TAK JE VSETKO OK
            else
                error(ERR_SEM_UNDEF,"Semanticka chyba! Parametre hlavicky funkcie definovanej cez forward a jej deklaracie sa nezhoduju\n");
        }       //  INAK NASTAVA CHYBA, BUD SA NEZHODUJU NAZVY IDENTIFIKATOROV NA ROVNAKOM INDEXE, ALEBO SA V DEKLARACNEJ HLAVICKE OBJAVIL VYSSI POCET PARAMETROV NEZ V DEFINICNEJ HLAVICKE
    
    
    if(newNode != NULL) error(ERR_SEM_UNDEF,"Semanticka chyba! Identifikator %s uz bol definovany\n", lexema->lex); // AK SA NIECO NASLO TAK SA REDEFINUJE -> ERROR
        
    newNode = BS_Add(pointers->SCOPE,lexema);  // INAK SA NIC NENASLO A DEKLARUJE SA
    newNode->data->value = malloc(sizeof(struct STerm));
    if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
    
   
    if(pointers->SYM_TABLE == pointers->SCOPE)  //JEDNA SA O DEKLARACIU GLOBALNEJ PREMENNEJ
        newNode->data->value->index = false;
    else{                                        //JEDNA SA O DEKLARACIU LOKALNEJ PREMENNEJ
        newNode->data->param = malloc(sizeof(char));
        if(newNode->data->param == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
        *(newNode->data->param) = pointers->PARAMCOUNT; // ULOZENIE INDEXU LOKALNEJ PREMENNEJ / PARAMETRA
        newNode->data->value->index = true;
    }
}


void SEM_funcDef(PTStructLex lexema){
       
    TSbinstrom newNode = BS_Find(pointers->SYM_TABLE, lexema);
    pointers->PARAMCOUNT = 0;
    
    if(newNode != NULL){
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNCTION) == 0)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Chyba pri definicii funkcie %s. Existuje premenna s rovnakym nazvom\n", lexema->lex);
        if((newNode->data->flags & LEX_FLAGS_TYPE_FUNC_DEK) != 0)
            error(ERR_SEM_UNDEF,"Semanticka chyba! Funkcia %s uz bola deklarovana\n", lexema->lex);
    }
    else {
        newNode = BS_Add(pointers->SYM_TABLE,lexema);
        newNode->data->value = malloc(sizeof(struct STerm));
        if(newNode->data->value == NULL) error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
        newNode->data->flags = LEX_FLAGS_TYPE_FUNCTION;
    }
    pointers->SCOPE = newNode;
}

void SEM_funcEnd(PTStructLex lexema){
    if(lexema->type == STREDNIK){
        pointers->SCOPE->data->flags = (pointers->SCOPE->data->flags | LEX_FLAGS_TYPE_FUNC_DEK);
        pointers->SCOPE->data->flags = (pointers->SCOPE->data->flags | LEX_FLAGS_TYPE_FUNC_DEF);
    }
    if(lexema->type == KEY_FORWARD) pointers->SCOPE->data->flags = (pointers->SCOPE->data->flags | LEX_FLAGS_TYPE_FUNC_DEF);
    
    pointers->SCOPE = pointers->SYM_TABLE;
}

/*
void SEM_createLeaf(ESyntaxRule pravidlo){

 *  FUNKCIA, KTORA VYTVARA LISTOVY UZOL PRE KAZDY PRVOK VYRAZU. TENTO UZOL OBSAHUJE UKAZATEL NA TERM, V KTOROM SU POTREBNE DATA PRE VYHODNOTENIE VYRAZU
 *  LISTOVY UZOL OBSAHUJE BUD ODKAZ NA TERM PRIAMO V TABULKE SYMBOLOV (GLOBALNE PREMENNE/LOKALNE PREMENNE A PARAMETRE), ALEBO PRIAMO TERM S HODNOTOU KONSTANTNOU.
 

    if(pravidlo == SRULE_ID){               // AK JE PRAVIDLO, REDUKUJ IDENTIFIKATOR NA 'E', TAK SA V KAZDOM PRIPADE HLADA POLOZKA V TABULKE SYMBOLOV
        
        TSbinstrom node = BS_Find(GLOBAL_scope, lexema);
        
        if((GLOBAL_scope != GLOBAL_sym_table)&&(node == NULL))                 
            node = BS_Find(GLOBAL_sym_table, lexema);      
            
        if (node == NULL)
            error(ERR_SEM,"Nedefinovana premenna '%s'", lexema->lex);
            
        if (flags & LEX_FLAGS_TYPE_FUNCTION != 0)                  // CHYBOVA HLASKA AK SA POUZIL IDENTIFIKATOR FUNKCIE
           error(ERR_SEM,"%s je identifikator funkcie", lexema->lex);
           
        if (flags & LEX_FLAGS_INIT == 0)                           // CHYBOVA HLASKA, VOLA SA NEINICIALIZOVANA PREMENNA
           error(ERR_SEM,"Premenna %s nebola inicializovana", lexema->lex);        
        
        SEM_pushSS(GLOBAL_SEM_ExpStack, node->data->value->type);        // ULOZENIE TYPU
        SEM_generate(OP_PUSH, node->data->value, NULL, NULL);
    }
    
    if(pravidlo == SRULE_CONST){                                     // V PRIPADE KONSTANTY JU POTREBUJEM ULOZIT DO STROMU AKO GENEROVANU PREMENNU   
        struct STerm * term = malloc(sizeof(struct STerm));          // PRE KONSTANTY SA VYTVARA NOVE UMIESTNENIE, A UKLADA SA DO ZOZNAMU KONSTANT
        
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
            
            default : break;
        }

        SEM_pushSS(GLOBAL_SEM_ExpStack, term->type);     // ULOZENIE TYPU NA SEMANTICKY ZASOBNIK
        SEM_generate(OP)
        SEM_addCL(term);                                 // ULOZENIE KONSTANTY DO ZOZNAMU KONSTANT
        return;
    }
 
 }

void SEM_createTree(ESyntaxRule pravidlo, E_OP wParam){
    
    ETermType typeRight = SEM_popSS(GLOBAL_SEM_ExpStack);
    ETermType typeLeft  = SEM_popSS(GLOBAL_SEM_ExpStack);
    
    if(typeLeft != typeRight)
        error(ERR_SEM, "Nekompatibilne datove typy.");
    
    GLOBAL_SEM_ReturnRegister->type = typeRight;
    
    SEM_generate(OP_POP, NULL, NULL, GLOBAL_SEM_SourceOne);
    SEM_generate(OP_POP, NULL, NULL, GLOBAL_SEM_SourceTwo);
    
    switch(pravidlo){
        case SRULE_PLUS     :
            SEM_generate(OP_PLUS, GLOBAL_SEM_SourceOne, GLOBAL_SEM_SourceTwo, GLOBAL_SEM_ReturnRegister);         
        break;                                                                          
        
        case SRULE_MINUS    :
            if(typeRight != TERM_STRING)
                SEM_generate(OP_MINUS, GLOBAL_SEM_SourceOne, GLOBAL_SEM_SourceTwo, GLOBAL_SEM_ReturnRegister);
            else 
                error(ERR_SEM, "Retazce nie je mozne odcitat");

        break;
        
        case SRULE_MUL      :
            if(typeRight != TERM_STRING)
                SEM_generate(OP_MUL, GLOBAL_SEM_SourceOne, GLOBAL_SEM_SourceTwo, GLOBAL_SEM_ReturnRegister);
            else 
                error(ERR_SEM, "Retazce nie je mozne nasobit");
        break;
        
        case SRULE_DIV      :
            if(typeRight != TERM_STRING)
                SEM_generate(OP_DIV, GLOBAL_SEM_SourceOne, GLOBAL_SEM_SourceTwo, GLOBAL_SEM_ReturnRegister);
            else 
                error(ERR_SEM, "Retazec nie je mozne delit retazcom");
        break;
        
        case SRULE_REL      :
                GLOBAL_SEM_ReturnRegister->type = TERM_BOOL;
                SEM_generate(wParam, GLOBAL_SEM_SourceOne, GLOBAL_SEM_SourceTwo, GLOBAL_SEM_ReturnRegister);           //!< TU POZOR NA WPARAM
        break;

        default             :   break;
        }     
    
        SEM_pushSS(GLOBAL_SEM_ExpStack, GLOBAL_SEM_ReturnRegister->type);
        SEM_generate(OP_PUSH, GLOBAL_SEM_ReturnRegister, NULL, NULL);
}

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


void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result){

 // FUNKCIA, KTORA VYTVORI TROJADRESNU INSTRUKCIU A PRIDA JU DO PROGRAMU
  
    P3AC newInstruction = malloc(sizeof(struct S3AC));
    newInstruction->op = operation;
    newInstruction->op1 = op1;
    newInstruction->op2 = op2;
    newInstruction->ret = result;
    
    SEM_addInstruction(newInstruction);
};
*/


