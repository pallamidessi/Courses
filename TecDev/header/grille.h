/**
 * \file       grille.h
 * \author     Pallamidessi joseph
 * \version    1.0
 * \date       4 mars 2012
 * \brief       header de grille.c 
 *
 * \details    Contient les prototypes des fonctions relatives au logigraphe(grille) ou a leurs traitements.
 *
 */
 
 
#ifndef grille_h
#define grille_h
#include<stdio.h>
#include<stdlib.h>
#define TAILLE_MAX 512

/**
 * \struct    str_grille
 * \brief     Structure contenant uniquement une matrice de char,avec un int indiquant le nombre de lignes et un int indiquant le nombre de colonnes  
 * \details   grille est un pointeur sur cette structure.
 */
 
typedef struct {
char ** matrice ;
int N;
int M;
} str_grille,*grille;


/**
 * \brief       Alloue la memoire pour la grille
 * \details     On initialise toute les cases de la matrice a '.' 
 *
 * @param     N int pour le nombre de lignes 
 * @param     M int pour le nombre de colonne
 * @return    L Le pointeur sur la grille alloue.
 */
 
grille alloue_grille(int,int);                  //allocation en memoire d'une grille N x M


/**
 * \brief       Affiche une grille avec un decalage X et Y
 * \details     Le decalage X et Y est calcul a partir des matrices des resultat des suites consecutives sur les ligne et les colonne pour avoir un affichage dynamique. 
 *                   
 * @param     l N M Une grille,et deux int  
 * @param     N Un int de decalage horizontal (X)
 * @param     M Un int de decalage vertical (X)
 */
 
void affiche_grille(grille,int,int);           //affiche une grille avec un decalage X et Y


/**
 * \brief       Libere la memoire d'un logigraphe.
 *
 * @param     l Le logigraphe a liberer.
 */

void desalloue_grille(grille);                 //free une grille



/**
 * \brief       Charge un fichier, et creer une grille suivant le contenu de ce fichier
 *
 * @param    l une grille deja existante pour obtenir la taille N x M 
 * @param    nom pointeur sur char pour le nom.
 * @param    mode pointeur sur char pour le mode.
 * @return   charge Le logigraphe nouvellement creer et initialiser selon le fichier.
 */
grille charger_grille(grille,char* ,char* );   //charge une grille depuis un fichier


/**
 * \brief       Compare deux grille
 * \details     Compare chaque valeurs une a une  
 *
 * @param     L1 une grille 
 * @param     L2 une grille
 * @return    I un int 0 pour vrai 1 pour faux.
 */
int compare(grille,grille);                 // compare la grille de l'utilisateur a la grille du fichier



/**
 * \brief       Compte les suite consecutive sur les colonnes
 * \details     Renvoi une grille des resultats des suite consecutive sur les colonne
 *
 * @param     L la grille a compter
 * @return    I la grille resultat
 */
grille compter_colonne(grille);             //grille des resultats des suite consecutive sur les colonne
/**
 * \brief       Compte les suite consecutive sur les lignes
 * \details     Renvoi une grille des resultats des suite consecutive sur les lignes
 *
 * @param     L la grille a compter
 * @return    I la grille resultat
 */
grille compter_ligne(grille);								//grille des resultats des suite consecutive sur les ligne


/**
 * \brief       coche/decoche toute une ligne du logigraphe
 * \details     Se base sur un table "interrupteur" pour savoir s'il faut cocher ou decocher une ligne donne
 *
 * @param     L la grille a modifier
 * @param     x  indice de la ligne a cocher
 * @param     tab_lig le tableau "interrupteur" 
 */
void cocher_ligne(grille ,int ,int* );      // coche/decoche une ligne  
/**
 * \brief       coche/decoche toute une colonne du logigraphe
 * \details     Se base sur un table "interrupteur" pour savoir s'il faut cocher ou decocher une colonne donne
 *
 * @param     L la grille a modifier
 * @param     y  indice de la colonne a cocher
 * @param     tab_col le tableau "interrupteur" 
 */
void cocher_colonne(grille ,int ,int* );    // coche/decoche une colonne  

/**
 * \brief       compte le decalage horizontal du a l'affichage des resultats des lignes
 * \details     A utiliser sur une grille resultat ligne
 *
 * @param     L la grille resultat ligne
 * @return    I un int de la longueur de la suite la pus grande dans la grille L
 */
int count_decalX(grille);                // compte le decalage du a l'affichage des resultats des lignes
/**
 * \brief       compte le decalage vertical du a l'affichage des resultats des colonne
 * \details     A utiliser sur une grille resultat colonne 
 *
 * @param     L la grille resultat colonne
 * @return    I un int de la longueur de la suite la pus grande dans la grille L
 */
int count_decalY(grille);									// compte le decalage du a l'affichage des resultats des colonnes
/**
 * \brief       Redondant : similaire a affiche_grille();
 * \details     A utiliser sur une grille resultats ligne,avec un decalage X et Y 
 *
 * @param     L la grille resultat ligne
 */
void afficheCountLigne(grille, int,int);   //affiche la grille des resultats des suite consecutive sur les ligne avec le decalage Y du a l'affichage des resultats des colonnes
/**
 * \brief       Redondant : similaire a affiche_grille();
 * \details     A utiliser sur une grille resultats colonne,avec un decalage X et Y 
 *
 * @param     L la grille resultat colonne
 */
void afficheCountCol(grille, int,int);     //affiche la grille des resultats des suite consecutive sur les colonne avec le decalage X du a l'affichage des resultats des lignes
/**
 * \brief       Grosse fonction : gere les entree utilisateur et les ecriture dans la grille a completer
 * \details     Entree : deplacement du curseur, cocher ligne/colonne, ecriture de '+' '.'
 *
 * @param     entree int de la valeur du caractere saisie
 * @param    tab_lig pointeur sur le tableau "interrupteur" ligne 
 * @param    tab_col pointeur sur le tableau "interrupteur" colonne
 * @param   x pointeur sur l'int de deplacement X dans la matrice 
 * @param   y pointeur sur l'int de deplacement Y dans la matrice
 * @param   decalX decalage X pour l'affichage de la position du curseur "O"
 * @param   decalY decalage Y pour l'affichage de la position du curseur "O"
 * @param   L1 grille de l'utilisateur
 * @param   L2 grille du jeu
 */
void deplacement(int ,int* ,int* ,int* ,int*,int,int,grille,grille );         //entree et deplacement  

#endif
