#ifndef _ARBQ1_H__
#define _ARBQ1_H__


#include"arbq.h"


/**
* /brief		Creer un Arbq correspondant a un damier de taille n carre
*	/details
*
*	@param	n Un int, la taille d'un cote du damier
* @return	a Un Arbq, L'Arbq ainsi creer
**/
Arbq damier(int n);


/**
* /brief		Creer un Arbq correspondant a la symetrie axial horizontale d'un Arbq par son
* milieu
*	/details 	Utilise une fonction symh1 auxilliaire qui gere la recursion 
*
*	@param 	a Un Arbq, l'arbre sur lequelle appliquer la symetrie
* @return r Un Arbq, resultat de la symetrie	
**/
Arbq symh(Arbq a);


/**
* /brief		Creer un Arbq correspondant a la symetrie axial horizontale d'un Arbq par son
* milieu
*	/details 	Utilise une fonction symh1 auxilliaire qui gere la recursion 
*
*	@param 	a Un Arbq, l'arbre sur lequelle appliquer la symetrie
* @return r Un Arbq, resultat de la symetrie	
**/
Arbq symv(Arbq a);


/**
* /brief		Creer un Arbq correspondant a la rotation a gauche d'un Arbq donne
*	/details
*
*	@param	a Un Arbq, l'arbre sur lequelle appliquer la rotation 
* @return	r Un Arbq, le resultat de la rotation 
**/
Arbq rotg(Arbq a);


/**
* /brief		Creer un Arbq correspondant a la rotation a droite d'un Arbq donne
*	/details
*
*	@param	a Un Arbq, l'arbre sur lequelle appliquer la rotation 
* @return	r Un Arbq, le resultat de la rotation 
**/
Arbq rotd(Arbq a);


/**
* /brief		La reduction d'un Arbq, en enlevant un fils sur 2
*	/details	Les fils enleve sur _ et _, et les pere avec seulement 2 fils sont
*	re-enraciner entre eux
*
*	@param 	a Un Arbq, l'arbre qu l'on veut reduire 
* @return	r Un Arbq, le resultat d la reduction 
**/
Arbq dzoo(Arbq a);


/**
* /brief		Fonction de parcours en pronfondeur d'un Arbq, en appliquant une fonction passe
* en parametre sur les feuilles
*	/details	Les fonction passees en argument on un prototype de la forme Arbq fonction(Arbq)
*
*	@param	a 				Un Arbq, sur lequelle on veut appliquer une fonction aux feuilles	
*	@param	operation Un pointeur de fonction, la fonction applique une transformation sur
*	les feuilles
* @return	r 				Un Arbq, resultat de la transformation de la fonctio0n passee en argument sur
* les feuilles de l'arbre passe en argument 
**/
Arbq parc(Arbq a,Arbq(*operation)(Arbq));


/**
* /brief		Inverse les couleurs d'une feuille de type Arbq
*	/details
*
*	@param	a	Un Arbq, une feuille sur laquelle on veut inverser ses couleurs
* @return	r Un Arbq, une nouvelle feuille avec les couleurs inverse da la feuille passe en
* argument 
**/
Arbq invc(Arbq a);


/**
* /brief		Donne le niveau de gris d'une feuille de type Arbq	
*	/details
*
*	@param	a	Un Arbq, une feuille dont on veut les niveaux de gris
* @return	r	Un Arbq, une nouvelle feuille avec le niveaux de gris de la feuille passe en
* argument 
**/
Arbq nivg(Arbq a);


/**
* /brief		Assombris les couleurs d'une feuille,si son niveau de gris associe est plus
* claire que le seuil donne  
*	/details	On assombris,si besoin, les couleurs par 50 
*
*	@param	a 		Un Arbq, une feuille que l'on veut seuiller 
*	@param	seuil Un int, le seuil qu'il faut utiliser
* @return	r			Un Arbq, une nouvelle feuille, assombri si besoin
**/
Arbq tresh(Arbq a,int seuil);


#endif 
