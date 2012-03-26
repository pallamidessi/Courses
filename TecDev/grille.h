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
void affiche_grille(grille,int,int);
void desalloue_grille(grille);
grille charger_grille(grille,char* ,char* );
int EstRempli(grille);
int compare(grille,grille);
grille compter_colonne(grille);
grille compter_ligne(grille);
void cocher_ligne(grille ,int ,int* );
void cocher_colonne(grille ,int ,int* );
void test2(grille);
int count_decalX(grille);
int count_decalY(grille);
void afficheCountLigne(grille, int,int);
void afficheCountCol(grille, int,int);
#endif
