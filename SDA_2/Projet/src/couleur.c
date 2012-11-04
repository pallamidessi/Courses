#include "couleur.h"


couleur nouv_couleur(){
	return NULL;	
}

couleur ic(int r,int v,int b){
	
	new_couleur=(couleur) malloc(sizeof(str_couleur));
	
	new_couleur->r=r;
	new_couleur->v=v;
	new_couleur->b=b;
	
	return new_couleur;
}


couleur r(couleur c){
	return c->r;
}

couleur v(couleur c){
	return c->v;
}

couleur b(couleur c){
	return c->b;
}

couleur blanc(){
	return ic(0,0,0);
}

couleur noir(){
	return ic(255,255,255);	
}


void detruire_couleur(couleur c){
	free(c);	
}
