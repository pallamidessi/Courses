
/*======================================================*\
  Wednesday September the 25th 2013
  Arash HABIBI
  Polygon.h
\*======================================================*/

#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Vector.h"

#define P_MAX_VERTICES 1000

typedef struct
{
	int _nb_vertices;
	Vector _vertices[P_MAX_VERTICES];
	int _is_closed;
	int _is_filled;
	int _is_convex;
} Polygon;

void P_init(Polygon *p);
// initialise un polygone (0 sommets)

void P_copy(Polygon *original, Polygon *copie);
// original et copie sont deux polygones déjà alloués.
// Cette fonction copie les donnée
// depuis original vers copie de façon à ce que les
// deux polygones soient identiques.

void P_addVertex(Polygon *P, Polygon pos);
// ajoute un sommet au polygone P. Ce nouveau sommet est situé en pos.

void P_removeLastVertex(Polygon *P);
// enlève le dernier sommet de P

void P_draw(Polygon *P);
// dessine le polygone P

void P_print(Polygon *P, char *message);
// Affiche sur une console les données de P
// à des fins de debuggage.

void  P_tournerAutourDeLAxeY(Polygon *P, double radians);
// tourne tous les points de P d'un angle de radians
// radians autour de l'axe Y.

#endif // __POLYGON_H__
