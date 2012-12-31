/**
 * \file			kruskal.h
 * \author		Pallamidessi Joseph
 * \version		1.0
 * \date			09/12/2012
 * \brief			header kruskal.h
 *
 * \details		Fonctions auxilliaire de la'lgoritme de kruskal et l'algorithme lui-meme
*/

#ifndef __TP1_H
#define __TP1_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef float E;
typedef int bool;

#define true 1
#define false 0

/**
*	/struct		str_matrix
*	/brief		Une structure contenant la taille de la matrice et une matrice
*	/details 	Il s'agit d'une matrice d'int,et on defini un pointeur sur la structure
*
**/
typedef struct matrix {
	E **mat;									/*<!	mat: La matrice de sorte E	*/
	int nb_rows;							/*<!	nm: La matrice d'int	*/
	int nb_columns;						/*<!	nm: La matrice d'int	*/
}str_matrix,*Matrix;				/*<!	mat: La matrice d'int	*/


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix newMatrix(int nb_rows,int nb_columns);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
E getElm(Matrix m,int row,int column);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
void setElm(Matrix m,int row,int column,E val);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
void deleteMatrix(Matrix m);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
int isSymetric(Matrix m);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
int isSquare(Matrix m);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix identite(int nb_rows,int nb_columns);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix transpose(Matrix m);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix addition(Matrix a,Matrix b);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix multiplication(Matrix a,Matrix b);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix mult_scalar(E scalar,Matrix m);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix copie(Matrix m);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
void affichage(Matrix m);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
void remplissage(Matrix m);
/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix saisie();

#endif
