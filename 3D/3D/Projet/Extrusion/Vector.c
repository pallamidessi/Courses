
/*======================================================*\
  Wednesday September the 25th 2013
  Arash HABIBI
  Vector.c
\*======================================================*/

#include "Vector.h"

//------------------------------------------------

Vector V_new(float x, float y, float z)
{
  Vector v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

//------------------------------------------------
// a des fin de debug

void V_print(Vector v, char *message)
{
  fprintf(stderr,"%s : %f %f %f\n",message, v.x,v.y,v.z);
}

Vector V_add(Vector v1, Vector v2){
	Vector new=V_new(0,0,0);

	new.x=v1.x+v2.x;
	new.y=v1.y+v2.y;
	new.z=v1.z+v2.z;

	return new;
}

Vector V_substract(Vector v1, Vector v2){
	Vector new=V_new(0,0,0);

	new.x=v1.x-v2.x;
	new.y=v1.y-v2.y;
	new.z=v1.z-v2.z;

	return new;
}

Vector V_multiply(double lambda, Vector v){
	Vector new=V_new(0,0,0);

	new.x=lambda*v.x;
	new.y=lambda*v.y;
	new.z=lambda*v.z;

	return new;
}


float V_dot(Vector v1, Vector v2){
	float result=0;

	result+=v1.x*v2.x;
	result+=v1.y*v2.y;
	result+=v1.z*v2.z;

	return result;
}


Vector V_cross(Vector v1, Vector v2){
	Vector new=V_new(0,0,0);

	new.x=v1.y*v2.z-v1.z*v2.y;
	new.y=v1.z*v2.x-v1.x*v2.z;
	new.z=v1.x*v2.y-v1.y*v2.x;

	return new;
}

int V_isOnTheRight(Vector M, Vector A, Vector B){
	Vector AB=V_substract(B,A);
	Vector result=V_cross(A,AB);
	
	if (result.z>=0) {
		return 1;		
	} 
	else{
		return 0;
	}
}

double V_length(Vector v){

	double length=sqrt(x*x+y*y+z*z);
	return lenght;
}

int V_segmentsIntersect(Vector p1, Vector p2, Vector q1, Vector q2){

	if (V_isOnTheRight(p1,q1,q2) && !V_isOnTheRight(p2,q1,q2) || 
			V_isOnTheRight(p2,q1,q2) && !V_isOnTheRight(p1,q1,q2)){
		return=1;
	}
}

int V_rayIntersectsSegment(Vector M, Vector u_ray, Vector p1, Vector p2){
	
	if(V_cross(u_ray,V_substract(p2,p1).z>0))
}

double V_decompose(Vector p, Vector u){
	return 
}

