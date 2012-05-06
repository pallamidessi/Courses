/**
 * \file       Image.h
 * \author     Pallamidessi joseph
 * \version    1.0
 * \date       4 mars 2012
 * \brief      header de Image.c
 *
 * \details    Contient les prototypes des fonctions relatives au chargement d'une image ppm en logigraphe.
 *
 */
#ifndef _grille_H
#define _grille_H 
#include<stdio.h>
#include<stdlib.h>
#include "grille.h"
/**
 * \brief       Creer une grille d'apres une image ppm.
 * \details     Si l'image fait plus de 70 pixel de large elle sera tronquee a 70 colonnes.
 *
 * @param     nom_du_fichier nom du fichier a charger,doit etrs dans le repertoire courant. 
 * @param     mode mode pour charger le fichier ,ici toujours "r".
 * @param     seuil  la valeurs moyenne des pixel de l'image, si un pixel vaut plus alors il est un "+",sinon c'est un ".",cf Seuil().
 * @return    L  une grille chargee d'apres l'image.
 */
grille charger_image( char* nom_du_fichier,char* mode,int seuil);
/**
 * \brief       Calcule la valeurs moyenne des pixels de l'image.
 * @param     nom_du_fichier le nom du fichier a charger.
 * @return    seuil un int qui est la moyenne des valeurs des pixels de l'image.
 */
int Seuil(char* nom_du_fichier);
#endif
