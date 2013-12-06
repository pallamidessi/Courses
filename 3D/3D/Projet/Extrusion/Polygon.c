#include "Polygon.h"
#include "utils.h"
void P_init(Polygon *p){
	p->_nb_vertices=0;
	p->_is_closed=FALSE;
	p->_is_filled=FALSE;
	p->_is_convex=TRUE;
}
// initialise un polygone (0 sommets)

void P_copy(Polygon *original, Polygon *copie){
	int i;

	copie->_nb_vertices=original->_nb_vertices;
	copie->_is_closed=original->_is_closed;
	copie->_is_filled=original->_is_filled;
	copie->_is_convex=original->_is_convex;

	for (i = 0; i < P_MAX_VERTICES; i++) {
		copie->_vertices[i]=(Vector)original->_vertices[i];
	}
}
// original et copie sont deux polygones déjà alloués.
// Cette fonction copie les donnée
// depuis original vers copie de façon à ce que les
// deux polygones soient identiques.

void P_addVertex(Polygon *P, Vector pos){
	int index;
	bool is_filled=P->_is_filled;
	
	if (P->_nb_vertices==0) {
		index=0;
	}
	else{
		index=P->_nb_vertices;
	}

	if (!is_filled) {
		P->_vertices[index]=(Vector)pos;
		P->_nb_vertices++;
	}
	
	if (P->_nb_vertices==P_MAX_VERTICES) {
		P->_is_filled=TRUE;
	}
}
// ajoute un sommet au polygone P. Ce nouveau sommet est situé en pos.

void P_removeLastVertex(Polygon *P){
	P->_nb_vertices--;

	if (P->_nb_vertices<0) {
		P->_nb_vertices=0;
	}
}
// enlève le dernier sommet de P

void P_draw(Polygon *P){
	Vector* tab=P->_vertices;
	int nb_vertices=P->_nb_vertices;
	bool is_closed=P->_is_closed;
	int i;

	Vector current;
	Vector current2;
	
	if (P->_is_convex) {
		//glColor rouge
		glColor3d(1,0,0);	
	}
	else{
		//glColor bleu
		glColor3d(0,0,1);	
	}

	if(is_closed){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);

		for (i = 0; i < nb_vertices ; i++) {
			current=(Vector) tab[i];  
			glVertex3f(current.x,current.y,current.z+325);
		}

		glEnd();
	}
	else{
		if (nb_vertices>1) {
			for (i = 0; i < nb_vertices-1; i++) {
				current=(Vector) tab[i];  
				current2=(Vector) tab[i+1];
				current.z++;
				current2.z++;
				drawLine(current,current2);
			}
		}
	}
}
// dessine le polygone P

void P_print(Polygon *P, char *message){
	Vector* tab=P->_vertices;
	int nb_vertices=P->_nb_vertices;
	Vector current;
	int i;

	for (i = 0; i < nb_vertices; i++) {
		current=(Vector) tab[i];  
		printf("point %d : x %f, y %f, z %f \n",i,current.x,current.y,current.z);
	}
}
// Affiche sur une console les données de P
// à des fins de debuggage.

void  P_tournerAutourDeLAxeY(Polygon *P, double radians){}
// tourne tous les points de P d'un angle de radians
// radians autour de l'axe Y.


Vector P_center(Polygon *P){
	int i;
	int x=0,y=0,z=0;

	Vector* vertices=P->_vertices;
	int nb_vertices=P->_nb_vertices;

	for (i = 0; i <nb_vertices; i++) {
		x+=vertices[i].x;
		y+=vertices[i].y;
		z+=vertices[i].z;
	}

	return V_new(x/nb_vertices,y/nb_vertices,z/nb_vertices);
}

int P_close(Polygon *P){
	
	if (!P->_is_closed ){
		P_addVertex(P,P->_vertices[0]);
		if(P_simple(P)){
			P->_is_convex=P_isConvex(P);
			P_removeLastVertex(P);
			P->_is_closed=TRUE;
			return TRUE;
		}
		else{
			P_removeLastVertex(P);
			P->_is_closed=FALSE;
			return FALSE;
		}
	}
	return FALSE;
}

Vector P_normal(Polygon *P){
	if (!P->_is_closed) {
		return V_new(0,0,0);
	}
	
	Vector* vertices=P->_vertices;
	
	Vector V1=V_substract(vertices[1],vertices[0]);
	Vector V2=V_substract(vertices[2],vertices[1]);

	Vector normal=V_cross(V1,V2);
	Vector normal_unit=V_multiply((double)(1./V_length(normal)),normal);
	
	return normal_unit;
}


void P_translate(Polygon *P, Vector trans){
	int i;
	Vector* vertices=P->_vertices;
	int nb_vertices=P->_nb_vertices;

	for (i = 0; i < nb_vertices; i++) {
		vertices[i].x+=trans.x;
		vertices[i].y+=trans.y;
		vertices[i].z+=trans.z;
	}

}

//Marche uniquement pour de s point avec z=0;
int P_isConvex(Polygon *P){
	int i;
	int nb_vertices=P->_nb_vertices;
	int signe;

	if (nb_vertices<3) {
		return TRUE;
	}
	
	Vector* vertices=P->_vertices;
	Vector V1;
	Vector V2;
	Vector crossProduct;

	V1=V_substract(vertices[1],vertices[0]);
	V2=V_substract(vertices[2],vertices[1]);
	crossProduct=V_cross(V1,V2);

	if(crossProduct.z>0)
		signe=1;
	else
		signe=0;

	for (i = 1; i < nb_vertices-2; i++) {
		V1=V_substract(vertices[i+1],vertices[i]);
		V2=V_substract(vertices[i+2],vertices[i+1]);
		crossProduct=V_cross(V1,V2);
		
		if(!((crossProduct.z>0 && signe==1) || (crossProduct.z<0 && signe==0)))
			return FALSE;
	}
	
	if(P->_is_closed){
		V1=V_substract(vertices[nb_vertices-1],vertices[nb_vertices-2]);
		V2=V_substract(vertices[0],vertices[nb_vertices-1]);
		crossProduct=V_cross(V1,V2);
		
		if(!((crossProduct.z>0 && signe==1) || (crossProduct.z<0 && signe==0)))
			return FALSE;
	}

	return TRUE;
}

int P_simple(Polygon *P){
	Vector* vertices=P->_vertices;
	int nb_vertices=P->_nb_vertices;
	Vector last_vertex=vertices[nb_vertices-1];
	Vector before_last_vertex=vertices[nb_vertices-2];
	int i;

	if (nb_vertices<3) {
		return TRUE;
	}

	for (i = 0; i < nb_vertices-2; i++) {
		if (V_segmentsIntersect(last_vertex,before_last_vertex,vertices[i],vertices[i+1])) {
			printf("faux !!\n");
			return FALSE;
		}
	}

	return TRUE;
}
//transform un vecteur de A dans B
Vector transform(Vector Ax,Vector Ay,Vector Az,Vector Bx,Vector By,Vector Bz,Vector P){

	double newX,newY,newZ;
	newX=V_dot(Bx,Ax)*P.x+V_dot(Bx,Ay)*P.y+V_dot(Bx,Az)*P.z;
	newY=V_dot(By,Ax)*P.x+V_dot(By,Ay)*P.y+V_dot(By,Az)*P.z;
	newZ=V_dot(Bz,Ax)*P.x+V_dot(Bz,Ay)*P.y+V_dot(Bz,Az)*P.z;

	return V_new(newX,newY,newZ);
}
void P_rotate(Vector a,Vector b,Vector center,Polygon* P){
	
	float V1z=a.z;
	float V2z=b.z;
	
	float V1x=a.x;
	float V2x=b.x;

	double phi=0,tau=0,mu=0;
	double phi1,phi2,tau1,tau2,mu1,mu2;
	int i;
	int nb_vertices=P->_nb_vertices;
	Vector* vertices=P->_vertices;
	
	b=V_unit(b);
	//repere perlin
	vector bx,by;
	V_uxUyFromUz(b,&bx,&by);

	

	//repere normal
	vector ax,ay;
	V_uxUyFromUz(a,&ax,&ay);
	
	double dcm[3][3];

	

	double angleA[3];
	double angleB[3];
	double angleC[3];
	//cosinus directeur
	angleA[0]=acos(V_dot(a,V_new(0,0,1))/V_length(a));
	angleA[1]=acos(V_dot(a,V_new(0,1,0))/V_length(a));
	angleA[2]=acos(V_dot(a,V_new(1,0,0))/V_length(a));

	angleB[0]=acos(V_dot(b,V_new(0,0,1))/V_length(b));
	angleB[1]=acos(V_dot(b,V_new(0,1,0))/V_length(b));
	angleB[2]=acos(V_dot(b,V_new(1,0,0))/V_length(b));
	
	//printf("Angle normal %f %f %f\n,",angleA[0],angleA[1],angleA[2]);
	//printf("Angle perlin %f %f %f\n,",angleB[0],angleB[1],angleB[2]);

	for (i = 0; i < 3; i++) {
		
		if (angleB[i]>angleA[i]) {
			angleC[i]=angleB[i]-angleA[i];
		}
		else
			angleC[i]=-(angleA[i]-angleB[i]);
		
		angleC[i]*=180./3.14159;
	}
	//angleC[0]=angleB[0]-angleA[0];
	//angleC[1]=angleB[0]-angleA[0];
	//angleC[0]=angleB[0]-angleA[0];

	//Vector angle=V_cross(V_new(angleC[2],angleC[1],angleC[0]),a);

/*
	a.x-=center.x;
	a.y-=center.y;
	a.z-=center.z;

	b.x-=center.x;
	b.y-=center.y;
	b.z-=center.z;

	//drawLine(V_new(325,325,1),V_new(a.x+325,a.y+325,a.z));
	//drawLine(V_new(325,325,1),V_new(b.x+325,b.y+325,b.z));
	a.z=0;
	b.z=0;

	//Angle de rotation autours de z
	if(V_length(a)*V_length(b)==0)
		phi=0;
	else{
		phi1=(180./3.14159)*acos(V_dot(a,V_new(1,0,0))/(V_length(a)));
		phi2=(180./3.14159)*acos(V_dot(b,V_new(1,0,0))/(V_length(b)));
		printf("phi1 %f\n",phi1);
		printf("phi2 %f\n",phi2);
		phi=phi1-phi2;
	}
	printf("phi %f\n",phi);
	a.z=V1z;
	b.z=V2z;
	
	a.x=0;
	b.x=0;
	

	//Vector projX=V_cross(V_new(1,0,0),V_cross(b,V_new(1,0,0)));
	//Angle de rotation autours de x
	if(V_length(a)*V_length(b)==0)
		tau=0;
	else{
		tau1=(180./3.14159)*acos((V_dot(a,V_new(0,1,0))/(V_length(a))));
		tau2=(180./3.14159)*acos((V_dot(b,V_new(0,1,0))/(V_length(b))));
		printf("tau1 %f\n",tau1);
		printf("tau2 %f\n",tau2);
		tau=tau1-tau2;
	}

	printf("tau %f\n",tau);
	
	
	a.x=V1x;
	b.x=V2x;

	a.y=0;
	b.y=0;
	

	//Angle de rotation autours de y
	if(V_length(a)*V_length(b)==0)
		mu=0;
	else{
		mu1=(180./3.14159)*acos(V_dot(a,V_new(1,0,0))/(V_length(a)));
		mu2=(180./3.14159)*acos(V_dot(b,V_new(1,0,0))/(V_length(b)));
		printf("mu1 %f\n",mu1);
		printf("mu2 %f\n",mu2);
		mu=mu1-mu2;
	}

	printf("mu %f\n",mu);
	
*/
	for (i = 0; i < nb_vertices; i++) {
		vertices[i]=V_translate(vertices[i],center,-1);
		vertices[i]=V_rotateUx(vertices[i],angleC[2]);
		//V_print(vertices[i],"vertix a transformer");
		vertices[i]=V_rotateUy(vertices[i],angleC[1]);	
		//V_print(vertices[i],"vertix  transformer");
		vertices[i]=V_rotateUz(vertices[i],angleC[0]);	
		vertices[i]=V_translate(vertices[i],center,1);
	}
	
}
