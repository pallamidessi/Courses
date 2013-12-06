
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

	double result= A.x*B.y
		+B.x*M.y
		+A.y*M.x
		-M.x*B.y
		-M.y*A.x
		-B.x*A.y;
	
	if (result>0){
		return 1;		
	} 
	else{
		return 0;
	}

}

double V_length(Vector v){

	double length=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
	return length;
}

Vector V_unit(Vector v){

	double  length=V_length(v);
	return V_new(v.x/length,v.y/length,v.z/length);
}
float det_from_vectors(Vector v1,Vector v2,Vector v3){
	return ((v1.x*v2.y*v3.z
				+ v2.x*v1.z*v3.y
				+ v1.y*v2.z*v3.x)
			- (v1.z*v2.y*v3.x
				+ v1.y*v2.x*v3.z
				+ v1.x*v3.y*v2.z) );
}

int V_segmentsIntersect(Vector p1, Vector p2, Vector q1, Vector q2){
	float s1,s2,s3,s4;
	
	p1.z=1;
	p2.z=1;
	q1.z=1;
	q2.z=1;
	s1=det_from_vectors(p1,p2,q1);
	s2=det_from_vectors(p1,p2,q2);
	s3=det_from_vectors(q1,q2,p1);
	s4=det_from_vectors(q1,q2,p2);

	p1.z=0;
	p2.z=0;
	q1.z=0;
	q2.z=0;
	/*Cas simple*/
	if (s1*s2<0 && s3*s4<0) {
		return TRUE;
	}
	return FALSE;

}

int V_rayIntersectsSegment(Vector M, Vector u_ray, Vector p1, Vector p2){

	//if(V_cross(u_ray,V_substract(p2,p1).z>0))
	return 0;
}

double V_decompose(Vector p, Vector u){
	return 0.; 
}

Vector V_recompose(double x, double y, double z, Vector u, Vector v, Vector w){
	Vector result=V_new(0,0,0);
	return result;
}

void V_uxUyFromUz(Vector u_z, Vector *u_x, Vector *u_y){
	Vector new=V_new(u_z.x+1,u_z.y+2,u_z.z+3);
	
	*u_x=V_cross(u_z,new);
	*u_x=V_unit(*u_x);

	*u_y=V_cross(u_z,*u_x);
	*u_y=V_unit(*u_y);
}

Vector V_rotateUx(Vector V,double angle){
	float newY= ((V.y)*cos(((float)(angle)*3.14159)/180.))  -  (V.z*sin(((float)(3.14159)*(float)(angle))/180.));
	float newZ= (V.z*cos(((float)(3.14159)*(float)(angle))/180.))    + ((V.y)*sin(((float)(3.14159)*(float)(angle))/180.));
	
	V.y=newY;
	V.z=newZ;
	
	return V;
	
}

Vector V_rotateUy(Vector V,double angle){
	float newX= ((V.x)*cos(((float)(angle)*3.14159)/180.))  +  (V.z*sin(((float)(3.14159)*(float)(angle))/180.));
	float newZ= (V.z*cos(((float)(3.14159)*(float)(angle))/180.))    - ((V.x)*sin(((float)(3.14159)*(float)(angle))/180.));
	
	V.x=newX;
	V.z=newZ;
	
	return V;
}

Vector V_rotateUz(Vector V,double angle){
	float newX= ((V.x)*cos(((float)(angle)*3.14159)/180.))  -  (V.y*sin(((float)(3.14159)*(float)(angle))/180.));
	float newY= (V.y*cos(((float)(3.14159)*(float)(angle))/180.))    + ((V.x)*sin(((float)(3.14159)*(float)(angle))/180.));
	
	V.x=newX;
	V.y=newY;

	return V;
}

Vector V_translate(Vector V,Vector translate,int mult){
	return V_new(V.x+translate.x*mult,V.y+translate.y*mult,V.z+translate.z*mult);
}
