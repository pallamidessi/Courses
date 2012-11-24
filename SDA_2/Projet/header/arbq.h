#ifndef _ARBQ_H__
#define _ARBQ_H__


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"couleur.h"
#include"bmpfile.h" //pour le type bool


/**
*	/struct		str_arbq
*	/brief		Un noeud d'un arbre quadernaire 
*	/details 	Il ya 4 pointeurs sur str_arbq pour les fils, et un type couleur, pour la
*	couleur du noeud
*
**/
typedef struct Arbq{
	struct Arbq* no;	/*<!	Pointeur sur le fils no	*/
	struct Arbq* ne;	/*<!	Pointeur sur le fils ne	*/
	struct Arbq* so;	/*<!	Pointeur sur le fils so	*/
	struct Arbq* se;	/*<!	Pointeur sur le fils se	*/
	Couleur c;				/*<!	Couleur du noeud	(NULL si noeud interne ou racine)	*/
}str_arbq,*Arbq;		/*<!	Arbq: pointeur sur str_arbq */

/**
* /brief		Cree une feuille d'une couleur  donnee
*	/details
*
*	@param	c Couleur de la feuille a creer
* @return	f Une feuille de type arbq 
**/
Arbq f(Couleur c);


/**
* /brief		Enracine 4 Arbq (arbre quadranaire)	
*	/details	Le noeud pere cree n'a pas de couleur  
*
*	@param	a Un Arbq (no)
*	@param	b Un Arbq (ne)
*	@param	c Un Arbq (so)
*	@param	d Un Arbq (se)
* @return	R La racine,un Arbq avec a,b,c,d comme fils 
**/
Arbq e(Arbq a,Arbq b,Arbq c,Arbq d);


/**
* /brief		Renvoie le sous-arbre no d'un Arbq donne
*	/details
*
*	@param	a L'Arbq dont on veut le sous-arbre
* @return	R Le sous-arbre no de a
**/
Arbq no(Arbq a);


/**
* /brief		Renvoie le sous-arbre ne d'un Arbq donne
*	/details	
*
*	@param	a L'Arbq dont on veut le sous-arbre
* @return	R Le sous-arbre ne de a
**/
Arbq ne(Arbq a);


/**
* /brief		Renvoie le sous-arbre so d'un Arbq donne
*	/details
*
*	@param	a L'Arbq dont on veut le sous-arbre
* @return	R Le sous-arbre so de a
**/
Arbq so(Arbq a);


/**
* /brief		Renvoie le sous-arbre se d'un Arbq donne
*	/details
*
*	@param	a L'Arbq dont on veut le sous-arbre
* @return	R Le sous-arbre se de a
**/
Arbq se(Arbq a);


/**
* /brief		La hauteur d'un arbre ou un sous-arbre depuis sa racine
*	/details
*
*	@param 	a Un Arbq, dont on veut la hauteur
* @return h Un int, la hauteur
**/
int hauteur(Arbq a);


/**
* /brief		Renvoie la couleur de la racine :d'un Arbq
*	/details
*
*	@param 	a L'Arbq dont on veut la couleur de sa racine
* @return	c Une couleur
**/
Couleur c(Arbq a);


/**
* /brief		Test si un Arbq est une feuille	
*	/details
*
*	@param	a L'arbq que l'on veut tester
* @return	t Un bool, resultat du test
**/
bool estf(Arbq a);


/**
* /brief		Nombre de feuille d'un arbre depuis sa racine
*	/details
*
*	@param 	a Un Arbq, dont on veut le nombre de feuille 	
* @return	I Un int, le nombre de feuille de l'Arbq
**/
int nf(Arbq a);


/**
* /brief		Renvoie une feuille selon ses coordonnees (y,x) dans un Arbq
*	/details	
*
*	@param	a Un Arbq,dns lequel on cherche la feuille
*	@param	x	Un int, qui correspond a l'ordonnee en partant du coin gauche surperieur	
*	@param	y	Un int, qui correspond a l'abcisse en partant du coin gauche surperieur	
* @return	f Un Arbq, la feuille de coordonnee(x,y)
**/
Arbq p(Arbq a,int x,int y);


/**
* /brief		Free la memoire d'un Arbq depuis sa racine
*	/details
*
*	@param	a Un Arbq, l'arbre a detruir
**/
void detruire_Arbq(Arbq a);

#endif
