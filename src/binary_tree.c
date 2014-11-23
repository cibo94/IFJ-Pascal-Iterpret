#include "inc.h"

TSbinstrom BS_New(void *data,uint64_t key){


 	TSbinstrom novy= (TSbinstrom) malloc(sizeof(struct Sbinstrom)); 	
 	if (novy==NULL){
 		error(ERR_INTERNAL,"Chyba alokacia pamate!\n");
 	}

 	novy->lptr=NULL;
 	novy->rptr=NULL;
 	novy->data=data;
 	novy->key=key;

 	return novy;
}

void BS_Add(TSbinstrom root,void *data,uint64_t key){

 	TSbinstrom pom=root;
 	TSbinstrom koren=root;
 	bool AKTUALIZE=false;
 	bool LEFT;


 	while (pom!=NULL){				//!< kym neprideme na vrchol stromu
 		if (key==pom->key){
 			AKTUALIZE=true;
 			//!<log("Aktualizacia v bin strome!\n");
 			break;
 		}
 		koren=pom;
 		if (key > pom->key){				//!< porovnavam kluce
 			pom=pom->rptr;				//!< ak je novy kluc ostro vacsi - doprava
 			LEFT=false;
 		}
 		else{
 			pom=pom->lptr;				//!< ak je novy kluc mensi alebo rovny - dolava
 			LEFT=true;
 		}
 	}

 	if(AKTUALIZE==true)
 		pom->data=data;
 	else{
 		if(LEFT==true)
 			koren->lptr = BS_New(data, key);						//!< pripojime novy uzol na volne miesto
 		else
 			koren->rptr = BS_New(data, key);
 	}
}


TSbinstrom BS_Find(TSbinstrom root, uint64_t key){

 	TSbinstrom pom=root;

 	while ((pom!=NULL) && (key!=pom->key)){	//!< kym neprideme na vrchol stromu alebo nenajdeme kluc
 		if (key > pom->key)						//!< porovnavam kluce
 			pom=pom->rptr;						//!< ak je hladany kluc ostro vacsi - doprava
 		else
 			pom=pom->lptr;						//!< ak je hladany kluc mensi alebo rovny - dolava
 	}
 	return pom;									//!< pratime uzol na ktorom sme skoncili, ci uz 
}												//!< hladany uzol alebo NULL

void BS_Free(TSbinstrom root){
	if (root == NULL)
		return;
	BS_Free(root->lptr);
	BS_Free(root->rptr);
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
            printf("%s  +-[%s, %llu]\n", sufix, ((PTStructLex)TempTree->data)->lex, (long long unsigned int)TempTree->key);
	    else
            printf("%s  +-[NULL, %llu]\n", sufix, (long long unsigned int)TempTree->key);
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



static uint64_t __jenkins_hash (char *key, uint64_t mask) {
    uint64_t hash, i;
    size_t len = strlen(key);
    for(hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash&mask;
}

uint64_t hash (char *Fkey, uint64_t Ppar, char *Vkey) {
    return 0 | (__jenkins_hash(Fkey, ((uint64_t)~0)>>(64-27))<<(64-27)) |
            ((Ppar&(((uint64_t)~0)>>54))<<(64-37)) |
         ((~__jenkins_hash(Vkey, ((uint64_t)~0)>>(64-27)))&(((uint64_t)~0)>>(64-27)));
}
