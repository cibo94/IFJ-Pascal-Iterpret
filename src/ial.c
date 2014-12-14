#include "inc.h"
extern PGLOB_DEST pointers;

static uint32_t hash (char *key) {
    uint32_t hash, i;
    size_t len = strlen(key);
    for(hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

TSbinstrom BS_New(PTStructLex data){


 	TSbinstrom novy= (TSbinstrom) malloc(sizeof(struct Sbinstrom)); 	
 	if (novy==NULL){
 		error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
 	}

 	novy->lptr=NULL;
 	novy->rptr=NULL;
    novy->loc_table=NULL;
 	novy->data=data;
 	novy->key=hash(data->lex);

 	return novy;
}

TSbinstrom BS_Add(TSbinstrom root,PTStructLex data){
/*
TSbinstrom pom_root;

    if (root != pointers->SYM_TABLE)
        pom_root=root->loc_table;
    else
        pom_root=root;
*/
// NIE som si uplne isty ci ich netreba vymenit, v tom pripade vsak ma valgrind problem s neinicializovanym LEFT
    if (root==NULL){
    TSbinstrom Uk=BS_New(data);
    if( pointers->SYM_TABLE==NULL){
        pointers->SYM_TABLE=Uk;
    }
    else
        pointers->CURRENTFUNCT->loc_table=Uk;
    pointers->SCOPE=Uk;

    return Uk;
    }

 	TSbinstrom pom=root;
 	TSbinstrom koren=root;
 	bool LEFT;
    uint32_t key = hash(data->lex);
    int c = 0;

    

 	while (pom!=NULL){				//!< kym neprideme na vrchol stromu
 		if (key==pom->key &&
           !(c=strcmp(data->lex, pom->data->lex))){
            error(ERR_SEM_UNDEF, "Pokus o redefiniciu: '%s'\n", data->lex);
 		}
 		koren=pom;
 		if (key > pom->key){        	//!< porovnavam kluce
 			pom=pom->rptr;				//!< ak je novy kluc ostro vacsi - doprava
 			LEFT=false;
 		} else {
 			pom=pom->lptr;				//!< ak je novy kluc mensi alebo rovny - dolava
 			LEFT=true;
 		}
 	}

    if(LEFT==true){
        koren->lptr = BS_New(data);
        return koren->lptr;				//!< pripojime novy uzol na volne miesto
    }
    else{
	    koren->rptr = BS_New(data);
        return koren->rptr;
    }
}


TSbinstrom BS_Find(TSbinstrom root, PTStructLex dat){
    uint32_t key = hash(dat->lex);
if (root == NULL)
    return NULL;
/*
    if (root != pointers->SYM_TABLE)
        root=root->loc_table;
 */
    TSbinstrom pom=root;
    int c;
    
    while (pom!=NULL){							//!< kym neprejdeme na koniec stromu
        if (key==pom->key && !(c=strcmp(dat->lex, pom->data->lex)))	//!< ak sa rovnaju kluce a aj stringy nasli sme hladany uzol
		return pom;  							
      	if (key > pom->key)                                             //!< porovnavam kluce
        	pom=pom->rptr;                                          //!< ak je hladany kluc ostro vacsi - doprava
        else
                pom=pom->lptr;                                          //!< ak je hladany kluc mensi alebo rovny - dolava       
    }
 	return NULL;							//!< na konci ak sa nic nenaslo vrati NULL 
}												


void BS_Free(TSbinstrom root){
	if (root == NULL)
		return;
	BS_Free(root->lptr);
	BS_Free(root->rptr);
	BS_Free(root->loc_table);
    if (root->data!= NULL){
        if (root->data->lex != NULL)
            free(root->data->lex);
        if (root->data->value != NULL) 
            if (root->data->value->type != TERM_EMB)
                free(root->data->value);
        if (root->data->param != NULL)
            free(root->data->param);
        free(root->data);
    }
	free(root);

}

static void Print_tree2(TSbinstrom TempTree, char* sufix, char fromdir) {
    if (TempTree != NULL) {
        char* suf2 = (char*) malloc(strlen(sufix) + 4);
        strcpy(suf2, sufix);
        if (fromdir == 'L'){
            suf2 = strcat(suf2, "  |");
            printf("%s\n", suf2);
	    }else
	        suf2 = strcat(suf2, "   ");
	    Print_tree2(TempTree->rptr, suf2, 'R'); 
        if (TempTree->data != NULL)
            printf("%s  +-[%s, %d]\n", sufix, ((PTStructLex)TempTree->data)->lex, TempTree->data->value->type);
	    else
            printf("%s  +-[NULL, %d]\n", sufix, TempTree->data->value->type);
        strcpy(suf2, sufix);
        if (fromdir == 'R')
	        suf2 = strcat(suf2, "  |");	
	    else
	        suf2 = strcat(suf2, "   ");
	    Print_tree2(TempTree->lptr, suf2, 'L');
	    if (fromdir == 'R') printf("%s\n", suf2);
	        free(suf2);
    }
}

static void Print_tree(TSbinstrom TempTree) {
    printf("Struktura binarniho stromu:\n");
    printf("\n");
    if (TempTree != NULL)
        Print_tree2(TempTree, "", 'X');
    else
        printf("strom je prazdny\n");
    printf("\n");
    printf("=================================================\n");
} 

void BS_Print(TSbinstrom root){
    Print_tree(root);
}

 void BS_checkFunction(TSbinstrom root){

    if (root==NULL)
        return;
    BS_checkFunction(root->lptr);
    BS_checkFunction(root->rptr);
    SEM_checkFunction(root->data);
}
static inline void arr_swap(char *frst, char *sec) {
    char pom = *sec;
    *sec = *frst;
    *frst = pom;
}
char *EMB_sort(char *in, int size) {
    char *out = malloc(size+1);                             // alokovanie navratoveho pola
    if (out == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!");
    memcpy(out, in, size);                                  // vytvorenie kopie
    for (int krok = size / 2; krok > 0; krok = krok / 2)    // Kazdy cyklus sa krok zmensi o polovicu
        for (int j = krok; j < size; j++) 
            for (int i = j-krok; i >= 0 && out[i] > out[i+krok]; i = i - krok) 
                                                            // kazdym cyklom sa porovnaju hodnoty obkrok dozadu
                arr_swap(&out[i], &out[i+krok]);            // a vymenia sa
    out[size] = '\0';                                       // ukoncovacia nula ... ta sa nezoradovala
    return out;
}

int EMB_find(char *s, char *search){
    int inputLength = EMB_length(s),                        // Dlzka vstupneho retazca
        searchLength = EMB_length(search),                  // Dlzka hladaneho retazca
        sInd = 1,                                           // Pozicia vo vstupe
        searchInd = 1,                                      // Pozicia v hladanom retazci
        r,                                                  // Pomocna premenna pre urcenie vektora spatnych indexov
        c = 0;                                              // Posledny index urcenia spatneho indexu

    char *fail = malloc(inputLength*sizeof(char)+1);          // Pole navratovych indexov v pripade neuspesneho hladania
    if (fail == NULL) error(ERR_INTERNAL, "Chyba alokacie pamete!");
    fail[0] = 0;

    while ((sInd <= inputLength) && (searchInd <= searchLength))
    {
        if (sInd != c){                                     // Urcenie vektora pre sInd prvok vstupneho retazca
            r = fail[(sInd+1)-2];
            while ((r>0) && (s[r-1] != s[(sInd+1)-2]))
                r = fail[r-1];
            fail[(sInd+1)-1] = (char)(r + 1);
            c = sInd;
        }

        if ((searchInd == 0) || (s[sInd-1] == search[searchInd-1])){
                                                            // Hladanie podretazca
            sInd++;
            searchInd++;
        }
        else
            searchInd = fail[searchInd-1];
    }
    free(fail);                                             // uvolnenie pamate

    if (searchInd > searchLength)                           // ukoncenie hladania, vratenie retazca
        return (sInd - searchLength);
    else
        return 0;
}

