/**
 * \file			couleur.c
 * \author		Pallamidessi Joseph
 * \version		1.0
 * \date			02/12/2012
 * \brief			src couleur.c
 *
 * \details		Fonctions associees a couleur.h
*/
#include "couleur.h"


Couleur nouv_Couleur(){
	return NULL;	
}

Couleur ic(unsigned char r,unsigned char v,unsigned char b){
	
	Couleur new_Couleur=(Couleur) malloc(sizeof(str_couleur));
	
	new_Couleur->r=r;
	new_Couleur->v=v;
	new_Couleur->b=b;
	
	return new_Couleur;
}


unsigned char r(Couleur c){
	return c->r;
}

unsigned char v(Couleur c){
	return c->v;
}

unsigned char b(Couleur c){
	return c->b;
}

Couleur blanc(){
	return ic(0,0,0);
}

Couleur noir(){
	return ic(255,255,255);	
}

void detruire_Couleur(Couleur c){
	free(c);	
}
