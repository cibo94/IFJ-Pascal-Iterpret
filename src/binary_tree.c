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

void BS_Print(TSbinstrom root){
	if (root == NULL)
		return;
	BS_Print(root->lptr);
	BS_Print(root->rptr);
	printf("%d\n",root->key);
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
    return (__jenkins_hash(Fkey, ((uint64_t)~0)>>(64-27))<<(63-27)) |
            ((Ppar&(((uint64_t)~0)>>54))<<(63-37)) |
         ((~__jenkins_hash(Vkey, ((uint64_t)~0)>>(64-26)))&(((uint64_t)~0)>>(64-26)));
}
