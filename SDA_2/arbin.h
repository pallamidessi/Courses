#ifndef __ARBIN_H
#define __ARBIN_H

#include<stdio.h>
#include<stdlib.h>

#define S int
#define bool int

typedef struct arbin{
struct arbin *ag;	
struct arbin *ad;	
S etiquette;
}str_arbin,*arbin;

S max(S a,S b);

arbin arbre_nouv();
arbin enracinement(arbin gauche,S x,arbin droit);
arbin arbre_gauche(arbin a);
arbin arbre_droit(arbin a);
S racine(arbin a);
bool vide(arbin a);
int hauteur(arbin a);
arbin extre_gauche(arbin a);
arbin extre_droite(arbin a);
bool feuille(arbin a);
int nbr_feuille(arbin a);
int nbr_noeud(arbin a);
int nbr_noeudInterne(arbin a);
bool ega_arbre(arbin a,arbin b);
void affiche(arbin a);
#endif	
