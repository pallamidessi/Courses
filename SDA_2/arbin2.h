#ifndef __ARBIN_H
#define __ARBIN_H

#include<stdio.h>
#include<stdlib.h>

#define S char*
#define bool int
#define FORMAT_S %s
typedef struct arbin{
struct arbin *ag;	
struct arbin *ad;	
S etiquette;
}str_arbin,*arbin;

int max(int a,int b);

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
void affiche_terme(arbin a);
void affiche_arbre(arbin a);
int evaluer(arbin a);
#endif	
