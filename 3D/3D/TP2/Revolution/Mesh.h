
/*======================================================*\
  Wednesday September the 25th 2013
  Arash HABIBI
  Mesh.h
\*======================================================*/

#ifndef __MESH_H__
#define __MESH_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include "Vector.h"
#include "Polygon.h"

#define M_MAX_QUADS 5000

typedef struct
{
	Vector _vertices[4];
} Quad;

Quad Q_new(Vector v1, Vector v2, Vector v3, Vector v4);

//--------------------------------------------

typedef struct
{
	int _nb_quads;
	Quad _quads[M_MAX_QUADS];
	int _is_filled;
	// int _is_smooth;
} Mesh;

void M_init(Mesh *M);
// initialise un Mesh (0 quads)

void M_addQuad(Mesh *M, Quad q);
// ajoute au Mesh le quadrilatère q

void M_draw(Mesh *M);
// dessine le Mesh M

void M_print(Mesh *M, char *message);
// Affiche sur une console les données
// relatives à M à des fins des debuggage.

void M_addSlice(Mesh *M, Polygon *P1, Polygon *P2);
// P1 et P2 sont supposés être des polygones ayant le même
// nombre N de sommets. Cette fonction ajoute à M les N quads
// obtenus en reliant les sommets de P1 à deux de P2.

void M_revolution(Mesh *M, Polygon *P, int nb_tranches);
// A partir d'un polygone P, cette fonction divise 2*pi en
// nb_tranches angles et ajoute à M les quads nécessaires pour
// réaliser une révolution de P autour de l'axe Y (cf figure 1).

#endif // __MESH_H__
