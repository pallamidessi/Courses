
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

typedef struct
{
	float x, y, z;
} Vector;

Vector V_new(float x, float y, float z);

void V_print(Vector v, char *message);
Vector V_add(Vector v1, Vector v2);
Vector V_substract(Vector v1, Vector v2);
Vector V_multiply(double lambda, Vector v);
Vector V_cross(Vector v1, Vector v2);
float  V_dot(Vector v1, Vector v2);
float  V_length(Vector v);
Vector V_unit(Vector v);
int    V_isOnTheRight(Vector M, Vector A, Vector B);
int    V_segmentsIntersect(Vector p1, Vector p2, Vector q1, Vector q2);
int    V_rayIntersectsSegment(Vector M, Vector u_ray, Vector p1, Vector p2);

Vector V_turnAroundY(Vector p, double angle);
Vector V_turnAroundZ(Vector p, double angle);
Vector V_projectOnPlane(Vector v, Vector normal);

double V_decompose(Vector p, Vector u); 
Vector V_recompose(double x, double y, double z, Vector u, Vector v, Vector w); 
void V_uxUyFromUz(Vector u_z, Vector *u_x, Vector *u_y); 


#endif // __VECTOR_H__
