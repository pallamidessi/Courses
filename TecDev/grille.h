#include<stdio.h>
#include<stdlib.h>
#ifndef grille_h
#define grille_h
#define TAILLE_MAX 512


typedef struct {
char ** matrice ;
int N;
int M;
} str_grille,*grille;



grille alloue_grille(int,int);
void affiche_grille(grille);
void desalloue_grille(grille);
grille charger_grille(grille,char* ,char* );
int EstRempli(grille);
int compare(grille,grille);
void compter_colonne(grille);
void compter_ligne(grille);
void cocher_ligne(grille ,int ,int );
void cocher_colonne(grille ,int ,int );
void test2(grille);
#endif
