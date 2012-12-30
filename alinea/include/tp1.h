#ifndef __TP1_H
#define __TP1_H

#include<stdio.h>
#include<stdlib.h>

typedef float E;
typedef int bool;

#define true 1
#define false 0

typedef struct matrix {
	E **mat;
	int nb_rows, nb_columns;
}str_matrix,*Matrix;


Matrix newMatrix(int nb_rows,int nb_columns);
E getElm(Matrix m,int row,int column);
void setElm(Matrix m,int row,int column,E val);
void deleteMatrix(Matrix m);

int isSymetric(Matrix m);
int isSquare(Matrix m);
Matrix identite(int nb_rows,int nb_columns);
Matrix transpose(Matrix m);
Matrix addition(Matrix a,Matrix b);
Matrix multiplication(Matrix a,Matrix b);
Matrix mult_scalar(E scalar,Matrix m);
Matrix copie(Matrix m);
void affichage(Matrix m);
void remplissage(Matrix m);
Matrix saisie();
#endif
