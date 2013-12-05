
/*======================================================*\
  Wednesday September the 25th 2013
  Arash HABIBI
  Vector.h
\*======================================================*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bool.h"
typedef struct
{
	float x, y, z;
} Vector;

Vector V_new(float x, float y, float z);
// retourne un vecteur de composantes x, y et z

void V_print(Vector v, char *message);
// affiche à l'écran les coordonnées de v + un message (debug)

Vector V_add(Vector v1, Vector v2);
// retourne v1+v2

Vector V_substract(Vector v1, Vector v2);
// retourne v1-v2

Vector V_multiply(double lambda, Vector v);
Vector V_cross(Vector v1, Vector v2);
float  V_dot(Vector v1, Vector v2);
double  V_length(Vector v);
Vector V_unit(Vector v);
int    V_isOnTheRight(Vector M, Vector A, Vector B);
int    V_segmentsIntersect(Vector p1, Vector p2, Vector q1, Vector q2);
int    V_rayIntersectsSegment(Vector M, Vector u_ray, Vector p1, Vector p2);
float det_from_vectors(Vector v1,Vector v2,Vector v3);
Vector V_turnAroundY(Vector p, double angle);
Vector V_turnAroundZ(Vector p, double angle);
Vector V_projectOnPlane(Vector v, Vector normal);

double V_decompose(Vector p, Vector u); 
Vector V_recompose(double x, double y, double z, Vector u, Vector v, Vector w); 
void V_uxUyFromUz(Vector u_z, Vector *u_x, Vector *u_y); 

Vector V_rotateUz(Vector v,int angle);
Vector V_rotateUx(Vector v,int angle);
Vector V_rotateUy(Vector v,int angle);
Vector V_translate(Vector V,Vector translate,int mult);
#endif // __VECTOR_H__
