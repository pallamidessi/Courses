/**
 * \file       grille.h
 * \author     Pallamidessi joseph
 * \version    1.0
 * \date       4 mars 2012
 * \brief       Header de grille_cairo.c 
 *
 * \details    Contient les prototypse des fonctions relatives au logigraphe(grille) ou a leurs traitements pour la  version cairo.
 *
 */
 
 
#ifndef grille_h
#define grille_h
#include<stdio.h>
#include<cairo.h>
#include<stdlib.h>
#define TAILLE_MAX 512 /**< nombre de charactere maximum par ligne,dans la lecture d'un fichier*/


/**
 * \struct    str_grille
 * \brief     Structure contenant uniquement une matrice de char,avec un int indiquant le nombre de lignes et un int indiquant le nombre de colonnes  
 * \details   grille est un pointeur sur cette structure.
 */
typedef struct {
char ** matrice ;				/*!< Matrice de caractere. */
int N;									/*!< Nombre de lignes. */
int M; 									/*!< Nombre de colonnes. */
} str_grille,*grille;   /*!< Pointeur sur la structure grille. */





/**
 * \brief       Alloue la memoire pour la grille
 * \details     On initialise toute les cases de la matrice a '.' 
 *
 * @param     N int pour le nombre de lignes 
 * @param     M int pour le nombre de colonne
 * @return    L Le pointeur sur la grille alloue.
 */
grille alloue_grille(int N,int M);                  //allocation en memoire d'une grille N x M




/**
 * \brief       Libere la memoire d'un logigraphe.
 *
 * @param     l Le logigraphe a liberer.
 */
void desalloue_grille(grille l);                 //free une grille



/**
 * \brief       Charge un fichier, et creer une grille suivant le contenu de ce fichier
 *
 * @param    l une grille deja existante pour obtenir la taille N x M 
 * @param    nom pointeur sur char pour le nom.
 * @param    mode pointeur sur char pour le mode.
 * @return   charge Le logigraphe nouvellement creer et initialiser selon le fichier.
 */
grille charger_grille(grille l,char* nom,char* mode);   //charge une grille depuis un fichier



/**
 * \brief       Compare deux grille
 * \details     Compare chaque valeurs une a une  
 *
 * @param     L1 une grille 
 * @param     L2 une grille
 * @return    I un int 0 pour vrai 1 pour faux.
 */
int compare(grille L1,grille L2);                 // compare la grille de l'utilisateur a la grille du fichier



/**
 * \brief       Compte les suite consecutive sur les colonnes
 * \details     Renvoi une grille des resultats des suite consecutive sur les colonne
 *
 * @param     L la grille a compter
 * @return    I la grille resultat
 */
grille compter_colonne(grille L);             //grille des resultats des suite consecutive sur les colonnes



/**
 * \brief       Compte les suite consecutive sur les lignes
 * \details     Renvoi une grille des resultats des suite consecutive sur les lignes
 *
 * @param     L la grille a compter
 * @return    I la grille resultat
 */
grille compter_ligne(grille L);								//grille des resultats des suite consecutive sur les ligne



/**
 * \brief       compte le decalage horizontal du a l'affichage des resultats des lignes
 * \details     A utiliser sur une grille resultat ligne
 *
 * @param     L la grille resultat ligne
 * @return    I un int de la longueur de la suite la pus grande dans la grille L
 */
int count_decalX(grille L);                // compte le decalage du a l'affichage des resultats des lignes



/**
 * \brief       compte le decalage vertical du a l'affichage des resultats des colonne
 * \details     A utiliser sur une grille resultat colonne 
 *
 * @param     L la grille resultat colonne
 * @return    I un int de la longueur de la suite la pus grande dans la grille L
 */
int count_decalY(grille L);									// compte le decalage du a l'affichage des resultats des colonnes



#endif
