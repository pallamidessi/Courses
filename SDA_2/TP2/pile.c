#include"pile.h"




pile pilenouv(){
	return NULL;
}

pile empiler(pile p,S x){
	
	pile nouv=(pile) malloc(sizeof(str_pile));
	nouv->suivant=p;
	
	return nouv;	
}

pile depiler(pile p){
	tmp=p->suivant;	
	free(p);
	return tmp;
}

S sommet(pile p){
	
	return p->x;	
}

void affiche infixe()
