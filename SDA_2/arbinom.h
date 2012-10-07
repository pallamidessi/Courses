#ifndef __ARBINOM_H
#define __ARBINOM_H

#include"arbin.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct sarbinom{
	arbin b;
}str_arbinom,*arbinom;


arbinom feuilleb(S x);
arbinom lien(arbinom a,arbinom b);
S racineb(arbinom a);
bool estFeuille(arbinom a);
arbinom premierFils(arbinom a);
arbinom autreFils(arbinom a);
int hauteurb(arbinom a);
int nbr_noeudb(arbinom a);
int nbr_noeudInterneb(arbinom a);
int nbr_feuilleb(arbinom a);
int nbr_noeuds_hauteur(arbinom a,int h);





#endif

