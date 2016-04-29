#ifndef _PILE_H_
#define _PILE_H_

#include<stdio.h>
#include<stdlib.h>
#include"arbint.h"

#define bool int 
#define S int 

typedef struct Pile{
	struct *Pile suivant;
	S x;
	}str_pile,*pile;

pile pilenouv();
pile empiler(pile p,S x);
pile depiler(pile p);
S sommet(pile p);
void infixe(arbint a,void (*fonc)(arbin))
void affiche(arbint a)




















#endif

