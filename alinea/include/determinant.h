/**
 * \file			kruskal.h
 * \author		Pallamidessi Joseph
 * \version		1.0
 * \date			09/12/2012
 * \brief			header kruskal.h
 *
 * \details		Fonctions auxilliaire de la'lgoritme de kruskal et l'algorithme lui-meme
*/

#ifndef __DETERMINANT_H
#define __DETERMINANT_H
#include"tp1.h"


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Matrix Extraction(Matrix m,int row,int column);


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
int Determinant(Matrix m);

#endif
