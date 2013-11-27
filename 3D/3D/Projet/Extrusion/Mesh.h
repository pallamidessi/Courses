
/*======================================================*\
  Wednesday September the 25th 2013
  Arash HABIBI
  Mesh.h
\*======================================================*/

#ifndef _MESH_H_
#define _MESH_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include "Vector.h"
#include "Polygon.h"
#include "Perlin.h"

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

Mesh* M_new();
void M_print(Mesh *P, char *message);

void M_addQuad(Mesh *P, Quad q);
void M_addSlice(Mesh *P, Polygon *p1, Polygon *p2);
void M_revolution(Mesh *P, Polygon *p1, int nb_slices);
void M_perlinExtrude(Mesh *QM, Polygon *p, int nb_slices);

void M_draw(Mesh *P);

#endif // _MESH_H_
