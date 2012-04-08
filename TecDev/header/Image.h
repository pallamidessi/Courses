/**
 * \file       Image.h
 * \author     Pallamidessi joseph
 * \version    1.0
 * \date       4 mars 2012
 * \brief      header de Image.c
 *
 * \details    Contient les prototype des fonction relative au chargement d'une image ppm en logigraphe.
 *
 */
#ifndef _grille_H
#define _grille_H 
#include<stdio.h>
#include<stdlib.h>
#include "grille.h"

grille charger_image( char* nom_du_fichier,char* mode,int seuil);
int Seuil(char* nom_du_fichier);
#endif
