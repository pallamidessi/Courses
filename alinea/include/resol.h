#ifndef __RESOL_H
#define __RESOL_H

#include"tp1.h"
#include"gauss.h"
#include"determinant.h"


typedef struct systeme{
	Matrix matrice;
	Matrix valeur;
}str_systeme,*Systeme;

Systeme newSystem(int nb_rows,int nb_columns);
void remplissage_systeme(Systeme s);
Systeme resolution(Systeme s);
void affichage_systeme(Systeme s);
void deleteSysteme(Systeme s);

void valeur_propre(Matrix m);
#endif


