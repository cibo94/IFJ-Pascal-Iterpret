#include "inc.h"

//  GLOBALNE PREMENNE

     TSemStack    GLOBAL_SEM_ExpStack;
     int          GLOBAL_SEM_localVarCount;
     PTStructLex  lexema;
     TSbinstrom   GLOBAL_scope;
     TSbinstrom   GLOBAL_sym_table;
     TTerm *      GLOBAL_SEM_funcEAX;
     TProgram     GLOBAL_SEM_Program;
     
//  FUNKCIE NAD ZASOBNIKOM

void SEM_initSS(TSemStack stack){
    stack = malloc(sizeof(struct SemStack));
    stack->top = NULL;
}

void SEM_pushSS(TSemStack stack, TASS uzol){
    TStackElem new = malloc(sizeof(struct StackElem));
    new->previous = stack->top;                     // VRCHOL SA STANE PREDCHODCOM NOVEHO VRCHOLU
    new->data = uzol;                               // NASTAVENIE OBSAHU PRVKU ZASOBNIKA
    stack->top = new;                               // NASTAVI SA NOVY VRCHOL
}

TASS SEM_popSS(TSemStack stack){
    if(stack->top == NULL)
        return NULL; // pravdepodobne syntakticka chyba ktora sa nenasla
        
    TASS returnTree = stack->top->data;            // ULOZENIE NAVRATOVEHO STROMU
    TStackElem toDelete  = stack->top;             // ULOZENIE PRVKU KTORY SA ZMAZE 
    stack->top = toDelete->previous;               // ZNIZENIE VRCHOLU ZASOBNIKA
    free(toDelete);                                // UVOLNENIE POPNUTEHO PRVKU 
    return returnTree;
}

void SEM_disposeSS(TSemStack stack){
    TStackElem toDelete;                   // POMOCNY PTR
    while(stack->top != NULL){             // KYM NIE JE ZASOBNIK PRAZDNY, TAK RUSI VRCHOL
        toDelete = stack->top;
        stack->top = toDelete->previous;
        free(toDelete);
    }
}

//  FUNKCIE, KTORE NAPLNAJU TABULKU SYMBOLOV



//  FUNKCIE NAD STROMOM

void SEM_ASSdispose(TASS tree){
    if(tree != NULL){
        disposeTree(tree->lptr);
        disposeTree(tree->rptr)
        if(tree->data != NULL)
            free(tree->data);
        free(tree);
    }
}


uint32_t SEM_TestIfDefined(PTStructLex item, TSbinstrom tree){
/**
 * FUNKCIA, KTORA SKONTROLUJE, CI BOL ITEM DEFINOVANY V SCOPE TREE ALEBO NA GLOBALNEJ UROVNI, AK ANO VRATI FLAGY NAJDENEHO UZLU V TS
 */
    TSbinstrom node = BS_Find(tree, item);
    if((tree != GLOBAL_sym_table)&&(node == NULL))                 
        node = BS_Find(GLOBAL_sym_table, item);       
    
    if (node == NULL)
        error(ERR_SEM,"Nedefinovany identifikator '%s'", item->lex);
    else
        return node->data->flags;
}


void SEM_createLeaf(ESyntaxRule pravidlo){
/**
 *  FUNKCIA, KTORA VYTVARA LISTOVY UZOL PRE KAZDY PRVOK VYRAZU. TENTO UZOL OBSAHUJE UKAZATEL NA TERM, V KTOROM SU POTREBNE DATA PRE VYHODNOTENIE VYRAZU
 *  LISTOVY UZOL OBSAHUJE BUD ODKAZ NA TERM PRIAMO V TABULKE SYMBOLOV (GLOBALNE PREMENNE/LOKALNE PREMENNE A PARAMETRE), ALEBO PRIAMO TERM S HODNOTOU KONSTANTNOU.
 */
    TASS newLeaf = malloc(sizeof(struct ASS));
    newLeaf->rptr = NULL;
    newLeaf->lptr = NULL;
    
    if(pravidlo == SRULE_ID){               // AK JE PRAVIDLO, REDUKUJ IDENTIFIKATOR NA 'E', TAK SA V KAZDOM PRIPADE HLADA POLOZKA V TABULKE SYMBOLOV
        
        uint32_t flags = SEM_TestIfDefined(lexema, GLOBAL_SCOPE)
      
        if (flags & LEX_FLAGS_TYPE_FUNCTION != 0)                  // CHYBOVA HLASKA AK SA POUZIL IDENTIFIKATOR FUNKCIE
            error(ERR_SEM,"%s je identifikator funkcie", lexema->lex);
        if (flags & LEX_FLAGS_INIT == 0)                           // CHYBOVA HLASKA, VOLA SA NEINICIALIZOVANA PREMENNA
            error(ERR_SEM,"Premenna %s nebola inicializovana", lexema->lex);        

        newLeaf->flags = flags;                                    // ULOZENIE FLAGOV, POTREBUJEM PRE TYPOVU KONTROLU V STROME 
        newLeaf->data = flags;                                     // NOVY UZOL BUDE ODKAZOVAT NA POLOZKU V TS, CI UZ GLOBALNU ALEBO LOKALNU
                                                                                       // GLOBALNE PREMENNE OBSAHUJU PRIAMO HODNOTU, LOKALNE OBSAHUJU INDEX NA ZASOBNIKU 
        SEM_pushSS(GLOBAL_SEM_ExpStack, newLeaf);                                      // ULOZENIE UZLU NA SEMANTICKY ZASOBNIK
        return;
    };
    
    
    if(pravidlo == SRULE_CONST){                                     // V PRIPADE KONSTANTY JU POTREBUJEM ULOZIT DO STROMU AKO GENEROVANU PREMENNU   
        struct STerm * term = malloc(sizeof(struct STerm));          // PRE KONSTANTY SA VYTVARA NOVE UMIESTNENIE, A TO PRIAMO V STROME VYRAZU
        
        switch(lexema->type){                                        // PODLA TYPU LEXEMY SA SKONVERTUJE STRING NA POZADOVANU HODNOTU
        
            case INT_CONST    :
                term->value.integer = atoi(lexema->lex);
                term->type = TERM_INT;
                newLeaf->flags = LEX_FLAGS_TYPE_INT | LEX_FLAGS_INIT;   // KONSTANTA SA BUDE TVARIT AKO PREMENNA DANEHO TYPU, KTORA JE INICIALIZOVANA
            break;
            
            case REAL_CONST   :
                term->value.real = atof(lexema->lex);
                term->type = TERM_REAL;
                newLeaf->flags = LEX_FLAGS_TYPE_REAL | LEX_FLAGS_INIT;
            break;
            
            case STRING_CONST :
                term->value.string = lexema->lex;
                term->type = TERM_STRING;
                newLeaf->flags = LEX_FLAGS_TYPE_STRING | LEX_FLAGS_INIT;
            break;
            
            case KEY_TRUE   :
                term->value.boolean = true;
                term->type = TERM_BOOL;
                newLeaf->flags = LEX_FLAGS_TYPE_BOOL | LEX_FLAGS_INIT;
            break;
            
            case KEY_FALSE  :
                term->value.boolean = false;
                term->type = TERM_BOOL;
                newLeaf->flags = LEX_FLAGS_TYPE_BOOL | LEX_FLAGS_INIT;
            break;
            
            default : break;
        }
        newLeaf->data = term;                         // PRIRADENIE HODNOTY UZLU    
        SEM_pushSS(GLOBAL_SEM_ExpStack, newLeaf);     // ULOZENIE UZLU NA SEMANTICKY ZASOBNIK
        return;
    };
    
    return; // AK SA SEM DOSTANE TAK FILIP ZLE ZADAL PARAMETER. VO VYRAZOCH VYTVARAJU LISTY LEN KONSTANTY A IDENTIFIKATORY PREMENNYCH 
            //(AK POCITAME S TYM, ZE VOLANIE FUNKCII NIE JE POVOLENE V RAMCI VYRAZOV, VTEDY SA TO NEMUSI RIESIT CEZ ASS, PRETOZE VIEM ZE BUDE FUNKCIA STAT SAMA NA PRAVEJ STRANE)
}

void SEM_createTree(ESyntaxRule pravidlo, TEnumLexem wParam){
    TASS rightTree = SEM_popSS(GLOBAL_SEM_ExpStack);                        //  NAJPRV SA POPNE PRAVY, LEBO SA DAL NESKOR NA ZASOBNIK
    TASS leftTree  = SEM_popSS(GLOBAL_SEM_ExpStack);
    
    if(leftTree->flags != rightTree->flags)                     //  KONTROLA TYPOVEJ KOMPATIBILITY
        error(ERR_SEM, "Nekompatibilne datove typy.");
        
        
    TASS newNode = malloc(sizeof(struct ASS));              //  MIESTO PRE NOVY UZOL
    newNode->lptr = leftTree;                               //  SPOJENIE UZLOV
    newNode->rptr = rightTree;
    newNode->data = malloc(sizeof(struct STerm));           //  PRIPRAVA MIESTA PRE VYSLEDOK
    newNode->flags = leftTree->flags;                       //  VYSLEDOK MA ROVNAKY TYP AKO LUBOVOLNY OPERAND
    
    if(newNode->flags & LEX_FLAGS_TYPE_INT != 0)            // NASTAVENIE SPRAVNEHO TYPU MEDZIVYSLEDKOVEHO TERMU 
        newNode->data->type = TERM_INT;
    if(newNode->flags & LEX_FLAGS_TYPE_REAL != 0)
        newNode->data->type = TERM_REAL;
    if(newNode->flags & LEX_FLAGS_TYPE_BOOL != 0)
        newNode->data->type = TERM_BOOL;
    if(newNode->flags & LEX_FLAGS_TYPE_STRING != 0)
        newNode->data->type = TERM_STRING;
        
    switch(pravidlo){
        case SRULE_PLUS     :
            SEM_generate(OP_PLUS, leftTree->data, rightTree->data, newNode->data);         //  GENEROVANIE 3AK  
        break;                                                                          
        
        case SRULE_MINUS    :
            if(newNode->flags & LEX_FLAGS_TYPE_STRING == 0)
                SEM_generate(OP_MINUS, leftTree->data, rightTree->data, newNode->data);
            else 
                error(ERR_SEM, "Retazce nie je mozne odcitat");

        break;
        
        case SRULE_MUL      :
            if(newNode->flags & LEX_FLAGS_TYPE_STRING == 0)
                SEM_generate(OP_MUL, leftTree->data, rightTree->data, newNode->data);
            else 
                error(ERR_SEM, "Retazce nie je mozne nasobit");
        break;
        
        case SRULE_DIV      :
            if(newNode->flags & LEX_FLAGS_TYPE_STRING == 0)
                SEM_generate(OP_DIV, leftTree->data, rightTree->data, newNode->data);
            else 
                error(ERR_SEM, "Retazec nie je mozne delit retazcom");
            break;
        
        case SRULE_REL      :
                newNode->data->type = TERM_BOOL;
                SEM_generate(wParam, leftTree->data, rightTree->data, newNode->data);           //!< TU POZOR NA WPARAM
            break;
        
        
        default             :   break;
        } 
}

void SEM_functionPrepare(int *paramCounter){    // PREROBIT DA SA TO KURVA JEDNODUCHSIE :D
/**
 *  VYSLEDOK FUNKCIE PO JEJ VOLANI JE NA GLOBALNEJ UROVNI, TEDA MA VLASTNOST GLOBALNEJ PREMENNEJ
 *  Z POHLADU SEMANTIKY A INTERPRETACIE SA HODNOTA VYSLEDKU UKLADA DO EAX
 */
    
    uint32_t flags = SEM_TestIfDefined(lexema, GLOBAL_sym_table)
   
    if(flags & LEX_FLAGS_TYPE_FUNCTION == 0)
        error(SEM_ERR,"%s nie je funkcia, ale premenna\n", lexema->lex);
    if(flags & LEX_FLAGS_TYPE_FUNC_DEK == 0)
        error(SEM_ERR,"Funkcia %s nie je deklarovana \n", lexema->lex);
        
    *paramCounter = 0;
    generate(OP_PUSH, GLOBAL_SEM_funcEAX, NULL, NULL);
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
/**
 *  VOLA SA PRI SPRACOVAVANI PARAMETROV VOLANEJ FUNKCIE
 *  V TERME FUNKCIE, KONKRETNE VO VALUE.STRING JE STRING OBSAHUJUCI PARAMETRE FUNKCIE
 */
 
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
    
    if(variableNode->flags & LEX_FLAGS_TYPE_INT != 0)            // NASTAVENIE SPRAVNEHO TYPU MEDZIVYSLEDKOVEHO TERMU 
        cTyp = 'i';
    if(variableNode->flags & LEX_FLAGS_TYPE_REAL != 0)
        cTyp = 'r';
    if(variableNode->flags & LEX_FLAGS_TYPE_BOOL != 0)
        cTyp = 'b';
    if(variableNode->flags & LEX_FLAGS_TYPE_STRING != 0)
        cTyp = 's';
    
    if(cTyp != functionNode->data->param[*paramCounter])
        error(ERR_SEM,"Argument %s je nespravneho typu", lexema->lex);
        
    generate(OP_PUSH, variableNode->data->value, NULL, NULL);
    *paramCounter++;
    return;
}

//  DALSIE FUNKCIE


void SEM_generate(E_OP operation, TTerm *op1, TTerm *op2, TTerm *result){
/**
 *  FUNKCIA, KTORA VYTVORI TROJADRESNU INSTRUKCIU A PRIDA JU DO PROGRAMU
 */ 
    P3AC newInstruction = malloc(sizeof(struct S3AC));
    newInstruction->op = operation;
    newInstruction->op1 = op1;
    newInstruction->op2 = op2;
    newInstruction->ret = result;
    
    SEM_addInstruction(newInstruction);
};



