#ifndef _COULEUR_H__
#define _COULEUR_H__

#include<stdio.h>
#include<stdlib.h>

typedef struct{
	unsigned char r;
	unsigned char v;
	unsigned char b;
}str_couleur,*couleur;

couleur nouv_couleur();
couleur ic(int r,int v,int b);
unsigned char r(couleur c);
unsigned char v(couleur c);
unsigned char b(couleur c);
couleur blanc();
couleur noir();
void detruire_couleur(couleur c);




#endif
