/**
 *	\file	iobmp.c
 *	\author	Jonathan Wonner
 *	\date	20121022
 *	\brief	Définition des méthodes pour importer/exporter des arbres quaternaires.
 */

#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "couleur.h"
#include "arbq.h"
#include "bmpfile.h"

/**
 *	Importe un fichier .mat.
 *	\param	filename	Le nom du fichier.
 *	\return	L'arbre construit à partir du fichier.
 */
Arbq import(char* filename)
{
	// On ouvre le fichier.
	FILE* file = fopen(filename, "r");
	int size;
	int tsize;
	int i, ll, cc;
	int r, g, b;
	Arbq** treeA;
	Arbq** treeB;
	
	// On récupère la taille de l'image.
	fscanf(file, "%d", &size);
	
	// On initialise une matrice de feuille.
	treeA = MALLOCN(Arbq*, size);
	for (i = 0; i < size; i++)
	{
		treeA[i] = MALLOCN(Arbq, size);
	}

	// On remplit les feuilles.
	for (ll = 0; ll < size; ll++)
	{
		for (cc = 0; cc < size; cc++)
		{
			fscanf(file, "%d %d %d", &r, &g, &b);
			treeA[ll][cc] = f(ic(r, g, b));
		}
	}

	tsize = size/2; 
	while (tsize != 0)
	{
		// On crée une matrice de noeuds quatre fois plus vite.
		treeB = MALLOCN(Arbq*, tsize);
		for (i = 0; i < tsize; i++)
			treeB[i] = MALLOCN(Arbq, tsize);
		
		// On remplit la mtrice.
		for (ll = 0; ll < tsize; ll++)
		{
			for (cc = 0; cc < tsize; cc++)
			{
				Arbq no = treeA[2*ll][2*cc];
				Arbq ne = treeA[2*ll][2*cc+1];
				Arbq so = treeA[2*ll+1][2*cc];
				Arbq se = treeA[2*ll+1][2*cc+1];
				treeB[ll][cc] = e(no, ne, so, se);
			}
		}
		
		treeA = treeB;
		tsize /= 2;
		// Et on recommence jusqu'à ce qu'il ne reste plus qu'une feuille.
	}
	return treeA[0][0];
}

/** 
 *	Transforme un arbre en matrice de feuilles.
 *	\param	aq	L'arbre à transformer.
 *	\return	La matrice de feuilles.
 */
Arbq** toLeafMatrix(Arbq aq)
{
	unsigned int imsize = ((unsigned int)sqrt(nf(aq)));
	unsigned int i, ll, cc;
	Arbq** matrix = MALLOCN(Arbq*, imsize);
	for (i = 0; i < imsize; i++)
		matrix[i] = MALLOCN(Arbq, imsize);

	for (ll = 0; ll < imsize; ll++)
	{
		for (cc = 0; cc < imsize; cc++)
		{
			matrix[ll][cc] = p(aq, ll, cc);
		}
	}
		
	return matrix;
}

/**
 *	Exporte un arbre en fichier .mat.
 *	\param	aq		L'arbre à exporter.
 *	\param	matname	Le nom du fichier .mat.
 */
void exportMatrix(Arbq aq, char* matname)
{
	unsigned int imsize = ((int unsigned)sqrt(nf(aq)));
	Arbq** leaves = toLeafMatrix(aq);
	unsigned int ll, cc;
	FILE* file = fopen(matname, "w");
	if (file == NULL) return;
	
	fprintf(file, "%d\n", imsize);
	for (ll = 0; ll < imsize; ll++)
	{
		for (cc = 0; cc < imsize; cc++)
		{
			Couleur coul = c(leaves[ll][cc]);
			fprintf(file, "%d %d %d\n", r(coul), v(coul), b(coul));
		}
	}
	
	fclose(file);
}

/**
 *	Exporte un arbre en fichier .bmp.
 *	\param	aq		L'arbre à exporter.
 *	\param	matname	Le nom du fichier .mat.
 */
void exportBmp(Arbq aq, char* bmpname)
{
	unsigned int imsize = ((int)sqrt(nf(aq)));
	Arbq** leaves = toLeafMatrix(aq);
	unsigned int ll, cc;
	bmpfile_t* bmp = bmp_create(imsize, imsize, 32);
	for (ll = 0; ll < imsize; ll++)
	{
		for (cc = 0; cc < imsize; cc++)
		{
			Couleur coul = c(leaves[ll][cc]);
			rgb_pixel_t pixel = {b(coul), v(coul), r(coul), 255};
			bmp_set_pixel(bmp, cc, ll, pixel);
		}
	}
	bmp_save(bmp, bmpname);
	bmp_destroy(bmp);
}


