/**
 * \file			kruskal.h
 * \author		Pallamidessi Joseph
 * \version		1.0
 * \date			09/12/2012
 * \brief			header kruskal.h
 *
 * \details		Fonctions auxilliaire de la'lgoritme de kruskal et l'algorithme lui-meme
*/

#ifndef __RESOL_H
#define __RESOL_H

#include"tp1.h"
#include"gauss.h"
#include"determinant.h"


typedef struct systeme{
	Matrix matrice;
	Matrix valeur;
}str_systeme,*Systeme;


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Systeme newSystem(int nb_rows,int nb_columns);


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
void remplissage_systeme(Systeme s);


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Systeme resolution(Systeme s);


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
void affichage_systeme(Systeme s);


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
void deleteSysteme(Systeme s);


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
Systeme saisie_systeme();


/**
* /brief		Creer un tableau d'arete d'apres une matrice d'adjacence
*	/details	Si la matrice est non-oriente (oriente==false),on copie uniquement les arete
*	de la tringulaire superieur prive de la diagonale.Sinon on copie toute les arete de al
*	matrice, prive de la diagonale.
*
*	@param	m la matrice dont ont veut extraire les aretes
* @return	a le tableau d'arete creer et initialiser
**/
void valeur_propre(Matrix m);

#endif


