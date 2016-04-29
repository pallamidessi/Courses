/**
 * \file			couleur.h
 * \author		Pallamidessi Joseph
 * \version		1.0
 * \date			02/12/2012	
 * \brief			header couleur.h
 *
 * \details		Definition de la structure couleur,avec ses fonctions generatrices
*/
#ifndef _COULEUR_H__
#define _COULEUR_H__

#include<stdio.h>
#include<stdlib.h>


/**
*	/struct	str_couleur
*	/brief	Un structure contenant un triplet definissant une couleur rgb 
*	/details Il s'agit d'un triplet de 3 unsigned char (0-255)
*
**/
typedef struct{
	unsigned char r;			/*<!	le champ correspondant la	valeur rouge 	*/
	unsigned char v;			/*<!	le champ correspondant la	valeur verte 	*/
	unsigned char b;			/*<!	le champ correspondant la	valeur bleu  	*/
}str_couleur,*Couleur;	/*<!	Couleur: pointeur sur la structure 			*/


/**
* /brief		Inititialise une nouvelle element de type couleur	
*	/details	Fait l'allocation memoire d'une couleur 
*
* @return	c Une Couleur, dont les valeurs sont pas initialisees	
**/
Couleur nouv_couleur();


/**
* /brief		Creer une nouvelle couleur, avec ses valeurs initialiser par les arguments
*	/details	Cette fonction appelle nouv_couleur
*
*	@param	r Un unsigned char, la valeur du champ r de la nouvelle couleur  
*	@param	v Un unsigned char, la valeur du champ v de la nouvelle couleur  
*	@param	b Un unsigned char, la valeur du champ b de la nouvelle couleur  
* @return c Une couleur, la couleur ainsi creer et initialiser	
**/
Couleur ic(unsigned char r,unsigned char v,unsigned char  b);


/**
* /brief		Renvoie la valeur du champ r de la couleur passee en argument 
*	/details	
*
*	@param	c Une couleur, dont on veut la valeur du champ r
* @return	r Un unsigned char, la valeur du champ r de la couleur c
**/
unsigned char r(Couleur c);


/**
* /brief		Renvoie la valeur du champ v de la couleur passee en argument 
*	/details	
*
*	@param	c Une couleur, dont on veut la valeur du champ v
* @return	r Un unsigned char, la valeur du champ v de la couleur c
**/


unsigned char v(Couleur c);
/**
* /brief		Renvoie la valeur du champ b de la couleur passee en argument 
*	/details	
*
*	@param	c Une couleur, dont on veut la valeur du champ b
* @return	r Un unsigned char, la valeur du champ b de la couleur c
**/


unsigned char b(Couleur c);

/**
* /brief		Creer une couleur, dont les valeurs correspondent a la couleurs blanche  	
*	/details	Soit le triplet (0,0,0)
*
* @return	c Une Couleur,dont les valeurs sont egale au triplet(0,0,0)	
**/


Couleur blanc();
/**
* /brief		Creer une couleur, dont les valeurs correspondent a la couleurs noire 
*	/details	Soit le triplet (255,255,255)
*
* @return	c Une Couleur,dont les valeurs sont egale au triplet(255,255,255)**/


Couleur noir();
/**
* /brief		Free une couleurs
*	/details
*
*	@param	c Une couleur, que l'on veut free
**/
void detruire_Couleur(Couleur c);


#endif
