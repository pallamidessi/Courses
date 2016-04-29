
/*======================================================*\
  Wednesday September the 25th 2013
  Arash HABIBI
  Polygon.h
\*======================================================*/

#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include "Vector.h"
#include "bool.h"

#define P_MAX_VERTICES 1000


typedef struct
{
	int _nb_vertices;
	Vector _vertices[P_MAX_VERTICES];
	int _is_closed;
	int _is_filled;
	int _is_convex;
} Polygon;

Polygon* P_new();
void P_init(Polygon *P);

void P_copy(Polygon *original, Polygon *copie);
// original et copie sont deux polygones déjà alloués.
// Cette fonction copie les donnée
// depuis original vers copie de façon à ce que les
// deux polygones soient identiques.

void P_addVertex(Polygon *P, Vector p);
void P_removeLastVertex(Polygon *P);

void P_print(Polygon *P, char *message); 
void P_draw(Polygon *P);

void drawRepereTest(Vector x,Vector y, Vector z,Vector center,int mod);
int P_close(Polygon *P);
// Ferme un polygone pas encore fini
int P_isConvex(Polygon *P);
int P_isOnTheLeftOfAllEdges(Polygon *P, Vector M);
int P_nbEdgesIntersectedByRay(Polygon *P, Vector M, Vector u_ray);
int P_isInside(Polygon *P, Vector M);
void P_turnAroundY(Polygon *P, double radians);

Vector P_center(Polygon *P);
Vector P_normal(Polygon *P); 
void P_scale(Polygon *P, double factor); 
void P_translate(Polygon *P, Vector trans);

void P_rotate(Vector a,Vector b,Vector center,Polygon* P);

void P_print(Polygon *P, char *message);
// Affiche sur une console les données de P
// à des fins de debuggage.

void  P_tournerAutourDeLAxeY(Polygon *P, double radians);
// tourne tous les points de P d'un angle de radians
// radians autour de l'axe Y.

int P_simple(Polygon *P);
//Test si un polygon est simple lors de sa construction
//Si le polygone n'est pas simple alors,on enleve le dernier vertex.
int P_isConvex(Polygon *P);
//Renvoye 1 si le polygone est convexe, 0 sinon
//Convexe par defaut <= 2 sommet

#endif // _POLYGON_H_
