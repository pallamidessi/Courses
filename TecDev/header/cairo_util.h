/**
 * \file       grille.h
 * \author     Pallamidessi joseph
 * \version    1.0
 * \date       4 mars 2012
 * \brief       Header de cairo_utils.c 
 *
 * \details    Contient les prototype des fonctions d'affichages du logigraphe(grille) pour la version cairo .
 *
 */



#ifndef _grille_util_h
#define _grille_util_h

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "grille_cairo.h"
#include "Image_cairo.h"



/**
 * \struct    str_rectangle
 * \brief     Structure definissant un rectangle.
 * \details     Structure contenant un int pour pour la valeurs l'abcisse du point en bas a gauche,un int pour pour la valeurs de l'ordonnee du point en bas a gauche,int int pour la largeurs du rectangle  et un int pour la hauteurs.  
 */
typedef struct _rectangle{
int x;          /*!< L'abcisse du point en bas a gauche. */
int y;          /*!< L'ordonnee du point en bas a gauche. */
int width;      /*!< Largeurs du rectangle. */
int height;     /*!< La hauteurs du rectangle */
} rectangle_t;



/**
 * \brief       Affichage du logigraphe et des effets visuels
 * \details     Affiche le logigraphe en tracant la grille,affichant les valeurs et les carres de selection en surbrillances 
 *
 * @param     surface la surface cairo sur laquelle on veut afficher le logigraphe. 
 * @param     l la grille a remplir, pour obtenir les informations sur le nombre les colonnes et lignes.
 * @param     Ligne grille contenant les les sommes des suites consecutives sur les lignes du logigraphe l.
 * @param     Col grille contenant les les sommes des suites consecutives sur les colonnes du logigraphe l.
 * @param     decalX indication pour decaler le logigraphe pour afficher correctement les valeurs de des ligne.
 * @param     decalY indication pour decaler le logigraphe pour afficher correctement les valeurs de des ligne.
 * @param     Sx position x du pointeur de la souris pour l'affichages des carres de selections et pour les changement de valeurs.
 * @param     Sy position y du pointeur de la souris pour l'affichages des carres de selections et pour les changement de valeurs.
 */
void Affiche_jeu(cairo_surface_t* surface,grille l,grille Ligne,grille Col,int decalX,int
decalY,int Sx,int Sy,int bouton);



/**
 * \brief       Affiche un carre en surbrillances rouge sur le logigraphe et recupere les entree utilisateurs
 * \details     L'affichage et la recuperation d'entree se fait d'apres la position de la souris.
 *
 * @param     mask contexte cairo sur lequel les carre doivent s'afficher 
 * @param     Sx position x du curseurs
 * @param     Sy position y du curseurs
 * @param     decalX  indication pour decaler les carre sur la droite pour les faire s'afficher correctement sur le logigraphe.
 * @param     decalY  indication pour decaler les carre vers le bas pour les faire s'afficher correctement sur le logigraphe.
 * @param     bouton Interrupteur, s'il est activee (0) on change la matrice a l'endroit ou le carre de selction s'affiche 
 */
void carreSelection(cairo_t* mask,int Sx,int Sy,grille l,int decalX,int decalY,int bouton);



/**
 * \brief       Trace la grille du logigraphe
 * \details     Les lignes sont noirs
 *
 * @param     mask contexte cairo sur lequel la grille doit s'afficher 
 * @param     M int pour le nombre de colonne
 * @param     decalX  indication pour decaler la grille sur la droite pour les faire s'afficher correctement sur le logigraphe.
 * @param     decalY  indication pour decaler la grille vers le bas pour les faire s'afficher correctement sur le logigraphe.
 */
void tracer_grille(cairo_t* mask,grille l,int decalX,int decalY);



/**
 * \brief       Affiche un menu de selection au debut du programme
 * \details     Permet de choisir entre un fichier texte et une image et de specifier un nom de fichier a utiliser
 *
 * @param     surface la surface cairo sur laquelle on veut afficher le menu. 
 * @param     e un evenement du serveur X.
 * @param    keysym un keysym.
 * @return   l la grille chargee.
 */
grille menu(cairo_surface_t* surface,XEvent e,Display* dpy,KeySym keysym);
#endif
