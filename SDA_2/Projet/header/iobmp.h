/**
 *	\file	iobmp.h
 *	\author	Jonathan Wonner
 *	\date	20121022
 *	\brief	Déclaration des méthodes pour importer/exporter des arbres quaternaires.
 */

#include "base.h"
#include "couleur.h"
#include "arbq.h"

/** Importe un fichier .mat. */
Arbq import(char* filename);

/** Transforme un arbre en matrice de feuilles. */
Arbq** toLeafMatrix(Arbq aq);

/** Exporte un arbre en fichier .mat. */
void exportMatrix(Arbq aq, char* matname);

/** Exporte un arbre en fichier .nmp. */
void exportBmp(Arbq aq, char* bmpname);
